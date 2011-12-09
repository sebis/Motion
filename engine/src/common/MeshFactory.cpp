#include "Mesh.h"
#include "Trace.h"

#include <cmath>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace Common
{
	namespace MeshFactory
	{
		Mesh * Sphere(glm::vec4 color, int segments)
		{
			std::vector<Mesh::vertex> vertices;

			// create bottom vertex
			Mesh::vertex bottom;
			bottom.color = color;
			bottom.normal = glm::vec3(0.0f, -1.0f, 0.0f);
			bottom.position = glm::vec3(0.0f, -1.0f, 0.0f);
			bottom.texcoord = glm::vec2(0.5f, 0.0f);
			vertices.push_back(bottom);

            int currentVertex = 0;

            for (int i = 0; i < segments - 1; i++)
            {
                // latitude
                float theta = float(((i + 1) * M_PI / segments) - M_PI * 0.5);
                float v = i / float(segments - 1);

				float dy = glm::sin(theta);
				float dxz = glm::cos(theta);

                for (int j = 0; j < segments; j++)
                {
                    // longitude
                    float phi = float(j * (2*M_PI) / segments);

					float dx = dxz * glm::cos(phi);
					float dz = dxz * glm::sin(phi);

					glm::vec3 normal(dx, dy, dz);

                    float u = j / (float)segments;

					Mesh::vertex vertex;
					vertex.color = color;
					vertex.normal = normal;
					vertex.position = normal;
					vertex.texcoord = glm::vec2(1 - u, v);
					vertices.push_back(vertex);

                    currentVertex++;
                }
            }

			Mesh::vertex top;
			top.color = color;
			top.normal = glm::vec3(0.0f, 1.0f, 0.0f);
			top.position = glm::vec3(0.0f, 1.0f, 0.0f);
			top.texcoord = glm::vec2(0.5f, 1.0f);
			vertices.push_back(top);

			std::vector<Mesh::vertex> vData;
			std::vector<unsigned int> iData;

			for (int i = 0; i < segments; i++)
            {
				vData.push_back(vertices[0]);
				vData.push_back(vertices[1 + i]);
				vData.push_back(vertices[1 + (i + 1) % segments]);
            }

            for (int i = 0; i < segments - 2; i++)
            {
                for (int j = 0; j < segments; j++)
                {
                    int nextI = i + 1;
                    int nextJ = (j + 1) % segments;

					vData.push_back(vertices[1 + i * segments + j]);
					vData.push_back(vertices[1 + nextI * segments + j]);
					vData.push_back(vertices[1 + i * segments + nextJ]);
					if (nextJ == 0) vData.back().texcoord.x = 1.0f - vData.back().texcoord.x;
                    
					vData.push_back(vertices[1 + i * segments + nextJ]);
					if (nextJ == 0) vData.back().texcoord.x = 1.0f - vData.back().texcoord.x;
					vData.push_back(vertices[1 + nextI * segments + j]);
					vData.push_back(vertices[1 + nextI * segments + nextJ]);
					if (nextJ == 0) vData.back().texcoord.x = 1.0f - vData.back().texcoord.x;
                }
            }

            for (int i = 0; i < segments; i++)
            {
				vData.push_back(vertices[vertices.size() - 1]);
				vData.push_back(vertices[vertices.size() - 2 - i]);
				vData.push_back(vertices[vertices.size() - 2 - (i + 1) % segments]);
            }

			for (unsigned i = 0; i < vData.size(); i++)
				iData.push_back(i);

			Mesh * mesh = new Mesh();
			mesh->setVertices(vData);
			mesh->setIndices(iData);
			return mesh;
		}

		Mesh * Plane(glm::vec4 color, int gridSize)
		{
			Mesh::vertex vData[] = {
				{ glm::vec3(-1.0f, 0, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, gridSize), color },
				{ glm::vec3(-1.0f, 0, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, 0), color },
				{ glm::vec3(1.0f, 0, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(gridSize, gridSize), color },
				{ glm::vec3(1.0f, 0, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(gridSize, 0), color },
			};

			unsigned int iData[] = {
				0, 1, 2,
				2, 1, 3
			};

			std::vector<Mesh::vertex> vertices(&vData[0], &vData[3]);
			std::vector<unsigned int> indices(&iData[0], &iData[5]);
			
			Mesh * mesh = new Mesh();
			mesh->setVertices(vertices);
			mesh->setIndices(indices);
			return mesh;
		}

		/*void PlaneMesh(int width, int length, std::vector<Mesh::vertex> & vData, std::vector<glm::uint> & iData, glm::vec4 color)
		{
			float dx = 1.0f/width;
			float dz = 1.0f/length;

			for (int x = 0; x < width; x++)
			{
				for (int z = 0; z < length; z++)
				{
					int offset = vData.size();

					Mesh::vertex v1 = { glm::vec3(x * dx, 0.0f, z * dz), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, 1), color };
					vData.push_back(v1);

					Mesh::vertex v2 = { glm::vec3(x * dx, 0.0f, (z+1) * dz), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, 0), color };
					vData.push_back(v2);

					Mesh::vertex v3 = { glm::vec3((x+1) * dx, 0.0f, z * dz), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1, 1), color };
					vData.push_back(v3);

					Mesh::vertex v4 = { glm::vec3((x+1) * dx, 0.0f, (z+1) * dz), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1, 0), color };
					vData.push_back(v4);

					//int offset = x * length + z;
					iData.push_back(offset + 0);
					iData.push_back(offset + 1);
					iData.push_back(offset + 2);
					iData.push_back(offset + 2);
					iData.push_back(offset + 1);
					iData.push_back(offset + 3);
				}
			}
		}*/

		void PlaneMesh(int width, int length, std::vector<Mesh::vertex> & vData, std::vector<glm::uint> & iData, glm::vec4 color)
		{
			float dx = 1.0f/width;
			float dz = 1.0f/length;

			for (int z = 0; z < length+1; z++)
			{
				for (int x = 0; x < width+1; x++)
				{
					Mesh::vertex v = { glm::vec3(x * dx, 0.0f, z * dz), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(x * dx, z * dz), color };
					vData.push_back(v);

					if (x < width && z < length)
					{
						iData.push_back((width+1)*(z)   + (x));
						iData.push_back((width+1)*(z+1) + (x));
						iData.push_back((width+1)*(z)   + (x+1));

						iData.push_back((width+1)*(z)   + (x+1));
						iData.push_back((width+1)*(z+1) + (x));
						iData.push_back((width+1)*(z+1) + (x+1));
					}
				}
			}
		}

		Mesh * PlaneMesh(glm::vec4 color, int width, int length)
		{
			std::vector<Mesh::vertex> vData;
			std::vector<glm::uint> iData;

			PlaneMesh(width, length, vData, iData, color);

			Mesh * mesh = new Mesh();
			mesh->setVertices(vData);
			mesh->setIndices(iData);
			return mesh;
		}

		Mesh * Cube(bool override_color, glm::vec4 color)
		{
			glm::vec4 front_color = override_color ? glm::vec4(1.0f, 0.65f, 0.0f, 1.0f) : color;
			glm::vec4 back_color = override_color ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : color;
			glm::vec4 right_color = override_color ? glm::vec4(0.0, 0.0f, 1.0f, 1.0f) : color;
			glm::vec4 left_color = override_color ? glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) : color;
			glm::vec4 top_color = override_color ? glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) : color;
			glm::vec4 bottom_color = override_color ? glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) : color;
			Mesh::vertex vData[] =
			{
				// front face 0,1,2,3
				{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0, 0), front_color },
				{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0, 0), front_color },
				{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0, 0), front_color },
				{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0, 0), front_color },

				// right face 4,5,6,7
				{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0, 0), right_color },
				{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0, 0), right_color },
				{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0, 0), right_color },
				{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0, 0), right_color },

				// back face 8, 9, 10, 11
				{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0, 0), back_color },
				{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0, 0), back_color },
				{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0, 0), back_color },
				{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0, 0), back_color },

				// left face 12, 13, 14, 15
				{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0, 0), left_color },
				{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0, 0), left_color },
				{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0, 0), left_color },
				{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0, 0), left_color },

				// top face 16, 17, 18, 19
				{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, 0), top_color },
				{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, 0), top_color },
				{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, 0), top_color },
				{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, 0), top_color },

				// bottom face 20, 21, 22, 23
				{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0, 0), bottom_color },
				{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0, 0), bottom_color },
				{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0, 0), bottom_color },
				{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0, 0), bottom_color },
			};

			unsigned int iData[] =
			{
				0, 1, 2, 2, 1, 3,
				4, 5, 6, 6, 5, 7,
				8, 9, 10, 10, 9, 11,
				12, 13, 14, 14, 13, 15,
				16, 17, 18, 18, 17, 19,
				20, 21, 22, 22, 21, 23,
			};

			std::vector<Mesh::vertex> vertices(&vData[0], &vData[24]);
			std::vector<unsigned int> indices(&iData[0], &iData[36]);

			Mesh * mesh = new Mesh();
			mesh->setVertices(vertices);
			mesh->setIndices(indices);
			return mesh;
		}

		Mesh * FromFile(std::string str, glm::vec4 color)
		{
			int num_vertices = 0;
			int num_triangles = 0;

			std::vector<Mesh::vertex> vData;
			std::vector<unsigned int> iData;

			// Load model from file
			std::string line;
			std::ifstream file(str.c_str());

			if (!file.is_open()) {
				Trace::error("MeshFactory: File \"%s\" not found!\n", str.c_str());
				return 0;
			}

			// read header
			while (file.good())
			{
				std::getline(file, line);

				if (line == "end_header") {
					break;
				}

				// Tokenize line
				std::vector<std::string> tokens;
				std::istringstream iss(line);
				std::copy(std::istream_iterator<std::string>(iss),
					std::istream_iterator<std::string>(),
					std::back_inserter<std::vector<std::string> >(tokens));

				if (tokens[0] == "element")
				{
					if (tokens[1] == "vertex") // size of vertex data is defined here
						sscanf(line.c_str(), "element vertex %d", &num_vertices);
					else if (tokens[1] == "face") // size of index data
						sscanf(line.c_str(), "element face %d", &num_triangles);
				}
			}

			// read vertex data from file
			for (int i = 0; file.good() && i < num_vertices; i++)
			{
				std::getline(file, line);
				double x, y, z;
				double nx, ny, nz;
				double s, t;
				sscanf(line.c_str(), "%lf %lf %lf %lf %lf %lf %lf %lf", &x, &y, &z, &nx, &ny, &nz, &s, &t);
		
				Mesh::vertex v;
				v.position = glm::vec3(x, y, z);
				v.normal = glm::vec3(nx, ny, nz);
				v.texcoord = glm::vec2(s, 1-t);
				v.color = color;
				vData.push_back(v);
			}

			// read index data from file
			for (int i = 0; file.good() && i < num_triangles; i++)
			{
				std::getline(file, line);
				int iCount;
				glm::uint i0, i1, i2, i3;
				sscanf(line.c_str(), "%d %d %d %d %d", &iCount, &i0, &i1, &i2, &i3);
				iData.push_back(i0);
				iData.push_back(i1);
				iData.push_back(i2);
				if (iCount == 4) {
					iData.push_back(i0);
					iData.push_back(i2);
					iData.push_back(i3);
					num_triangles++;
				}
		
				// get vertices corresponding to this face
				/*Mesh::vertex& v0 = vData[i0];
				Mesh::vertex& v1 = vData[i1];
				Mesh::vertex& v2 = vData[i2];
		
				// add face normal to all face vertices
				glm::vec3 normal = glm::normalize(glm::cross(v2.position - v0.position, v1.position - v0.position));
				
				v0.normal += normal;
				v1.normal += normal;
				v2.normal += normal;*/
			}
	
			file.close();
	
			// normalize sum normal vectors
			/*for (int i = 0; i < num_vertices; i++) {
				vData[i].normal = glm::normalize(vData[i].normal);
			}*/

			Mesh * mesh = new Mesh();
			mesh->setVertices(vData);
			mesh->setIndices(iData);
			return mesh;
		}
	};	
} /* namespace Common */
