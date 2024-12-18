#include "Framebuffer.h"
#include "Model.h"


//void Model::Draw(Framebuffer& framebuffer, const glm::mat4& model, const class Camera& camera)
//{
//	//
//}

void Model::Update() 
{
	for (size_t i = 0; i < m_local_vertices.size(); i++) 
	{
		m_vb[i] = m_transform * glm::vec4{m_local_vertices[i], 1};
	}
}

bool Model::Load(const std::string& filename)
{
	// open file using ifstream (input file stream)
	std::ifstream stream(filename);
	// check if stream is_open
	if (!stream.is_open())
	{
		// TODO: report error to cerr and return false
		std::cerr << "stream not open" << filename << std::endl;
		return false;
	}

	vertexbuffer_t vertices;
	std::string line;
	while (std::getline(stream, line))
	{
		// read in vertex positions
		// https://cplusplus.com/reference/string/string/substr/
		if (line.substr(0, 2) == "v ")
		{
			// read position of vertex
			std::istringstream sstream{ line.substr(2) };
			glm::vec3 position;
			sstream >> position.x >> position.y >> position.z;

			// TODO: add position to vertices vector
			vertices.push_back(position);

		}
		// read in faces (triangles)
		else if (line.substr(0, 2) == "f ")
		{
			// read face (triangle), index of vertex position in vertices array [1, 2, 3]
			std::istringstream sstream{ line.substr(2) };
			std::string str;
			// https://en.cppreference.com/w/cpp/string/basic_string/getline
			while (std::getline(sstream, str, ' '))
			{
				std::istringstream sstream(str);
				std::string indexString;

				// read each triangle index (position, normal, uv)
				size_t i = 0;
				unsigned int index[3] = { 0, 0, 0 }; // 0 = position, 1 = normal, 2 = uv
				while (std::getline(sstream, indexString, '/'))
				{
					if (!indexString.empty())
					{
						std::istringstream indexStream{ indexString };
						indexStream >> index[i];
					}
					i++;
				}

				// check if index 0 (position) is valid
				if (index[0] != 0 && index[0] - 1 < vertices.size())
				{
					// get vertex at index position
					// index is 1 based, need to subtract one for array
					glm::vec3 position = vertices[index[0] - 1];

					m_vb.push_back(position);
				}
			}
		}
	}
	stream.close();
	
	return true;
}

bool Model::Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance)
{
	// check cast ray with mesh triangles 
	for (size_t i = 0; i < m_vb.size(); i + 3)
	{
		float t;
		if (Triangle::Raycast(ray, m_vb[i], m_vb[i + 1], m_vb[i + 2], minDistance, maxDistance, t))
		{
			return true;
		}
	
	}

	return false;
}

//void Model::SetColor(const color_t& newColor)
//{
//	m_color = newColor;
//}

