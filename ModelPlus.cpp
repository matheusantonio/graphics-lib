#include "ModelPlus.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
using namespace cguff;

// Funções para coons
vec3 c0(float s){
    return {15*cos(s), 10*sin(0.1*s), 0.8*sin(6*s+2.5)};
    //return -s*s+3;
}

//float c1(float s){
vec3 c1(float s){
    return {15*cos(s+0.2), 10*sin(0.1*s+1), -0.332917*sin(6*s+2.5)};
    //return cos(s);
}

//float d0(float t){
vec3 d0(float t){
    return {15*cos(0.2*t), 10*sin(t), 0.478778*cos(2*t)};
    //return -t*t-3;
}

//float d1(float t){
vec3 d1(float t){
    return {15*cos(0.2*t+1), 10*sin(t+0.1), 2*(1-t)*t+0.63879*cos(2*t)};
    //return 2*t*t;
}

vec3 Lc(float s, float t){
    return (1-t)*c0(s) + t*c1(s);
}

vec3 Ld(float s, float t){
    return (1-s)*d0(t) + s*d1(t);
}

//float b(float s, float t){
vec3 b(float s, float t){
    return (1-s)*(1-t)*c0(0) + s*(1-t)*c0(1) + (1-s)*t*c1(0) + s*t*c1(1);
}
//=======================================


void load_texture_data(GLenum target, const char* filename){
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	
	int format[] = {0, GL_RED, GL_RG, GL_RGB, GL_RGBA};
	glTexImage2D(target, 0, GL_RGBA, width, height, 0, 
		format[nrChannels], GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

unsigned int loadTexture(const char* filename){
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	stbi_set_flip_vertically_on_load(true);
	load_texture_data(GL_TEXTURE_2D, filename);
	glGenerateMipmap(GL_TEXTURE_2D); 
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}

unsigned int loadCubemap(const char* texture_faces[6]){
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	
	for(int i = 0; i < 6; i++)
		load_texture_data(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, texture_faces[i]);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

	return textureID;
}


void check_texture(std::string map_name, int texture, int texUnit){
	if(texture != -1){
		glActiveTexture(GL_TEXTURE0 + texUnit);
		glBindTexture(GL_TEXTURE_2D, texture);
		setUniform(map_name, texUnit);
		setUniform("has_" + map_name, 1);
	}else
		setUniform("has_" + map_name, 0);
}

float max(float a, float b){
	return a>b? a: b;
}

void useMaterial(Material mat){
	setUniform("material.ambient", mat.ambient);
	setUniform("material.diffuse", mat.diffuse);
	setUniform("material.specular", mat.specular);
	setUniform("material.shininess", max(1, mat.shininess));

	check_texture("diffuse_map", mat.texture_diffuse, 0);
	check_texture("specular_map", mat.texture_specular, 1);
	check_texture("normal_map", mat.texture_normal, 2);
}

void draw_mesh(Mesh M){
	useMaterial(M.mat);
	glBindVertexArray(M.vao);
	glDrawElements(GL_TRIANGLES, M.Ni, GL_UNSIGNED_INT, 0);
}

void draw_model(Model& M, mat4 ModelMatrix){
	ModelMatrix = ModelMatrix*M.ModelMatrix;

	setUniform("Model", ModelMatrix);

	for(int i = 0; i < M.meshes.size(); i++)
		draw_mesh(M.meshes[i]);
	
	for(int i = 0; i < M.children.size(); i++)
		draw_model(M.children[i], ModelMatrix);
}

Mesh generateMesh(Vertex V[], int N, unsigned int indices[], int Ni){
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, N*sizeof(Vertex), V, GL_STATIC_DRAW);
	
	int stride = sizeof(Vertex);
	unsigned long offset = 0;
	
	VertexAttribPointer("Vertex", 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	offset += sizeof(vec3);
	VertexAttribPointer("TexCoord", 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	offset += sizeof(vec2);
	VertexAttribPointer("Normal", 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	offset += sizeof(vec3);
	VertexAttribPointer("Tangent", 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	offset += sizeof(vec3);
	VertexAttribPointer("Bitangent", 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	
	GLuint element_buffer;
	glGenBuffers(1, &element_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Ni*sizeof(unsigned int), indices, GL_STATIC_DRAW);

	Mesh M = {vao, Ni};
	return M;
}

Material processMaterial(aiMaterial* aiMat){
	Material mat;

	aiColor3D color;
	aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	mat.diffuse = {color.r, color.g, color.b};
	
	aiMat->Get(AI_MATKEY_COLOR_SPECULAR, color);
	mat.specular = {color.r, color.g, color.b};
	
	aiMat->Get(AI_MATKEY_COLOR_AMBIENT, color);
	mat.ambient = {color.r, color.g, color.b};

	aiMat->Get(AI_MATKEY_SHININESS, mat.shininess);

	mat.texture_diffuse = -1;
	aiTextureType type = aiTextureType_DIFFUSE;
	if(aiMat->GetTextureCount(type) > 0){
		aiString Path;
		aiMat->GetTexture(type, 0, &Path);
		mat.texture_diffuse = loadTexture(Path.C_Str());
	}
	
	mat.texture_specular = -1;
	type = aiTextureType_SPECULAR;
	if(aiMat->GetTextureCount(type) > 0){
		aiString Path;
		aiMat->GetTexture(type, 0, &Path);
		mat.texture_specular = loadTexture(Path.C_Str());
	}
	
	mat.texture_normal = -1;
	type = aiTextureType_HEIGHT;
	if(aiMat->GetTextureCount(type) > 0){
		aiString Path;
		aiMat->GetTexture(type, 0, &Path);
		mat.texture_normal = loadTexture(Path.C_Str());
	}
	return mat;
}

std::ostream& operator<<(std::ostream& out, vec3 p){
	out << p.x << ' ' << p.y << ' ' << p.z << '\n';
	return out;
}

Mesh processMesh(aiMesh* mesh, const aiScene* scene){
	const int N = mesh->mNumVertices;
	Vertex V[N];

	for(int i = 0; i < mesh->mNumVertices; i++){
		aiVector3D P = mesh->mVertices[i];
		V[i].position = {P.x, P.y, P.z};

		aiVector3D N = mesh->mNormals[i];
		V[i].normal = {N.x, N.y, N.z};

		if(mesh->mTangents){
			aiVector3D T = mesh->mTangents[i];
			V[i].tangent = {T.x, T.y, T.z};
		}
		
		if(mesh->mBitangents){
			aiVector3D B = mesh->mBitangents[i];
			V[i].bitangent = {B.x, B.y, B.z};
		}

		if(mesh->mTextureCoords[0]){
			aiVector3D T = mesh->mTextureCoords[0][i];
			V[i].texCoord = {T.x, T.y};
		}
	}

	const int Nf = mesh->mNumFaces;
	const int Ni = 3*Nf;
	unsigned int indices[Ni];

	int k = 0;
	for(int i = 0; i < Nf; i++){
		aiFace face = mesh->mFaces[i];
		for(int j = 0; j < 3; j++){
			indices[k] = face.mIndices[j];
			k++;
		}
	}

	Mesh M = generateMesh(V, N, indices, Ni);
	M.mat = processMaterial(scene->mMaterials[mesh->mMaterialIndex]);

	return M;
}

Model processNode(aiNode *node, const aiScene *scene){
	Model model;

	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			model.ModelMatrix.M[i][j] = node->mTransformation[i][j];

	// process all the node's meshes (if any)
	model.meshes.resize(node->mNumMeshes);
	for(unsigned int i = 0; i < node->mNumMeshes; i++){
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
		model.meshes[i] = processMesh(mesh, scene);			
	}


	model.children.resize(node->mNumChildren);
	// then do the same for each of its children
	for(unsigned int i = 0; i < node->mNumChildren; i++)
		model.children[i] = processNode(node->mChildren[i], scene);

	return model;
}

Model load_model(const char* filename){
	std::cout << "model " << filename << '\n';
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filename, 
			aiProcess_GenSmoothNormals | 
			aiProcess_Triangulate |
			aiProcess_CalcTangentSpace 
			); 

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		printf("ERROR::ASSIMP::%s\n", importer.GetErrorString());
		Model model;
		model.ModelMatrix = loadIdentity();
		return model;
	}

	return processNode(scene->mRootNode, scene);
}

Material standard_material(){
	Material mat;
	mat.ambient = {0.2, 0.2, 0.2, 1.0};
	mat.diffuse = {0.8, 0.8, 0.8, 1.0};
	mat.specular = {0.8, 0.8, 0.8, 1.0};
	mat.shininess = 50;

	mat.texture_diffuse = -1;
	mat.texture_specular = -1;
	mat.texture_normal = -1;

	return mat;
}

Mesh sphere_mesh(int m, int n, Material mat){
	const int N = m*n;
	Vertex P[N];
	
	float u0 = 0, u1 = M_PI;
	float du = (u1-u0)/(m-1);
	
	float v0 = 0, v1 = 2*M_PI;
	float dv = (v1-v0)/(n-1);

	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			float u = u0 + i*du;
			float v = v0 + j*dv;
		
			int ij = i + j*m;
			P[ij].position = {sin(u)*cos(v), sin(u)*sin(v), cos(u)};
			P[ij].texCoord = {i/(m-1.0f), j/(n-1.0f)};
			P[ij].normal = normalize(P[ij].position);
			vec3 Su = {cos(u)*cos(v), cos(u)*sin(v), -sin(u)};
			vec3 Sv = {-sin(u)*sin(v), sin(u)*cos(v), 0};
			P[ij].tangent = normalize(Su);
			P[ij].bitangent = normalize(Sv);
		}
	}
	
	const int Ni = 6*(m-1)*(n-1);
	unsigned int indices[Ni];
	int k = 0;
	for(int i = 0; i < m-1; i++){
		for(int j = 0; j < n-1; j++){	
			int ij = i + j*m;
			indices[k++] = ij;
			indices[k++] = ij+1;
			indices[k++] = ij+m;
			
			indices[k++] = ij+m+1;
			indices[k++] = ij+m;
			indices[k++] = ij+1;
		}
	}
	
	Mesh M = generateMesh(P, N, indices, Ni);
	M.mat = mat;
	return M;
}

Model sphere_model(int nu, int nv, Material mat){
	Model M;
	M.meshes.push_back(sphere_mesh(nu, nv, mat));
	M.ModelMatrix = loadIdentity();
	return M;
}

Mesh surface_mesh(int m, int n, Material mat, float s){
	int N = m*n;
	Vertex P[N];

	float u0 = -5, u1 = 5;
	float du = (u1-u0)/(m-1);
	
	float v0 = -5, v1 = 5;
	float dv = (v1-v0)/(n-1);

	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			float u = u0 + i*du;
			float v = v0 + j*dv;
		
			int ij = i + j*m;
			P[ij].position = {u, v, cos(s) + sin(u*v/4)};
			
			vec3 Su = {1, 0, cos(u*v/4)*v/4};
			vec3 Sv = {0, 1, cos(u*v/4)*u/4};
			
			P[ij].normal = normalize(cross(Su, Sv));
			P[ij].texCoord = {i/(m-1.0f), j/(n-1.0f)};
			P[ij].tangent = normalize(Su);
			P[ij].bitangent = normalize(Sv);
		}
	}
	int Ni = 6*(m-1)*(n-1);
	unsigned int indices[Ni];
	
	int k = 0;
	for(int i = 0; i < m-1; i++){
		for(int j = 0; j < n-1; j++){	
			int ij = i + j*m;
			indices[k++] = ij;
			indices[k++] = ij+1;
			indices[k++] = ij+m;
			
			indices[k++] = ij+m+1;
			indices[k++] = ij+m;
			indices[k++] = ij+1;
		}
	}

	Mesh M = generateMesh(P, N, indices, Ni);
	//Materia: M.mat = mat;
	Material material;
	material.ambient = {0.0,0.0,0.0};
	material.diffuse = {0.043, 0.211, 0.278};
	material.specular = {0.0, 0.0, 0.0};
	material.shininess = 0;
	material.texture_diffuse=loadTexture("oceantex.jpeg");
	material.texture_normal = -1;
	material.texture_specular = -1;
	M.mat = material;
	return M;
}

Model surface_model(int m, int n, float s, Material mat){
	Model M;
	M.meshes.push_back(surface_mesh(m, n, mat, s));
	M.ModelMatrix = loadIdentity();
	return M;
}

Mesh coons_mesh(int m, int n, Material mat){
	int N = m*n;
	
	Vertex P[N];

	float u0 = 0, u1 = 1;
	float du = (u1-u0)/(m-1);
	
	float v0 = 0, v1 = 1;
	float dv = (v1-v0)/(n-1);

    for(int i =0;i<m;i++){
        for(int j=0;j<n;j++){
            float u = u0 + i*du;
            float v = v0 + j*dv;
            int ij = i + j*m;
            P[ij].position = Lc(u, v) + Ld(u, v) - b(u, v);
            
			vec3 Su = {1, 0, cos(u*v/4)*v/4};
			vec3 Sv = {0, 1, cos(u*v/4)*u/4};
			
			P[ij].normal = normalize(cross(Su, Sv));
			P[ij].texCoord = {i/(m-1.0f), j/(n-1.0f)};
			P[ij].tangent = normalize(Su);
			P[ij].bitangent = normalize(Sv);
			
        }
    }

    int Ni = 6*(m-1)*(n-1);
    unsigned int indices[Ni];
    int k=0;
    for(int i = 0; i < m-1; i++){
		for(int j = 0; j < n-1; j++){	
			int ij = i + j*m;
            
			indices[k++] = ij;
			indices[k++] = ij+1;
			indices[k++] = ij+m;
			
			indices[k++] = ij+m+1;
			indices[k++] = ij+m;
			indices[k++] = ij+1;       
		}
    }
    Mesh M = generateMesh(P, N, indices, Ni);
	Material material;
	material.ambient = {0.0,0.0,0.0};
	material.diffuse = {0.121, 0.317, 0.043};
	material.specular = {0.0, 0.0, 0.0};
	material.shininess = 0;
	material.texture_diffuse=loadTexture("islandtex.jpg");
	material.texture_normal = -1;
	material.texture_specular = -1;
	M.mat = material;

    return M;
}

Model coons_model(int m, int n, Material mat){
	Model M;
	M.meshes.push_back(coons_mesh(m, n, mat));
	M.ModelMatrix = loadIdentity();
	return M;
}
