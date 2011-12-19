#include "Mesh.h"
#include "Trace.h"
#include "Utils.h"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace Common
{
	namespace MeshFactory
	{
		namespace
		{
			bool g_useStaticDraw = false;
		}

		void setStaticDraw(bool useStaticDraw)
		{
			g_useStaticDraw = useStaticDraw;
		}

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

			Mesh * mesh = new Mesh(g_useStaticDraw);
			mesh->setVertices(vData);
			mesh->setIndices(iData);
			return mesh;
		}

		Mesh * Cylinder(int segments, float radius, const glm::vec4 & color)
		{
			std::vector<Mesh::vertex> vertices;
			std::vector<unsigned int> indices;

			// create bottom vertices
			
			unsigned c1 = 2*segments;
			unsigned c2 = c1+1;

			for (int i = 0; i < segments; i++)
			{
				float ratio = (float)i / (float)segments;
				float angle = float(2.0f*M_PI*ratio);

				float x = radius * std::cos(angle);
				float z = radius * std::sin(angle);

				Mesh::vertex bottom;
				bottom.position = glm::vec3(x, 0.0f, z);
				bottom.normal = glm::vec3(x, 0.0f, z);
				bottom.color = color;
				bottom.texcoord = glm::vec2(ratio, 1.0f);
				vertices.push_back(bottom);

				Mesh::vertex top;
				top.position = glm::vec3(x, 1.0f, z);
				top.normal = glm::vec3(x, 0.0f, z);
				top.color = color;
				top.texcoord = glm::vec2(ratio, 0.0f);
				vertices.push_back(top);

				unsigned i1 = i*2;
				unsigned i2 = i1+1;
				unsigned i3 = ((i+1)*2)%(segments*2);
				unsigned i4 = i3+1;

				indices.push_back(i1);
				indices.push_back(i3);
				indices.push_back(c1);

				indices.push_back(i2);
				indices.push_back(i4);
				indices.push_back(c2);

				indices.push_back(i1);
				indices.push_back(i2);
				indices.push_back(i3);

				indices.push_back(i2);
				indices.push_back(i3);
				indices.push_back(i4);
			}

			Mesh::vertex bottom;
			bottom.position = glm::vec3(0.0f, 0.0f, 0.0f);
			bottom.normal = glm::vec3(0.0f, -1.0f, 0.0f);
			bottom.color = color;
			bottom.texcoord = glm::vec2(0.0f, 0.0f);
			vertices.push_back(bottom);

			Mesh::vertex top;
			top.position = glm::vec3(0.0f, 1.0f, 0.0f);
			top.normal = glm::vec3(0.0f, 1.0f, 0.0f);
			top.color = color;
			top.texcoord = glm::vec2(1.0f, 1.0f);
			vertices.push_back(top);

			Mesh * mesh = new Mesh(g_useStaticDraw);
			mesh->setVertices(vertices);
			mesh->setIndices(indices);
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

			std::vector<Mesh::vertex> vertices(&vData[0], &vData[4]);
			std::vector<unsigned int> indices(&iData[0], &iData[6]);
			
			Mesh * mesh = new Mesh(g_useStaticDraw);
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
			float dx = 5.0f/width;
			float dz = 5.0f/length;

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

			Mesh * mesh = new Mesh(g_useStaticDraw);
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

			Mesh * mesh = new Mesh(g_useStaticDraw);
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

			Mesh * mesh = new Mesh(g_useStaticDraw);
			mesh->setVertices(vData);
			mesh->setIndices(iData);
			return mesh;
		}

		Mesh * MeshFactory::Terrain(int size, float textureSize, glm::vec4 color)
		{
			Utils::Random r;

			Mesh::Vertices vData;
			Mesh::Indices iData;

			float dx = 1.0f/size;
			float dz = 1.0f/size;

			// intialize data
			float * data = new float[size*size];
			for (int i = 0; i < size; i++)
				for (int j = 0; j < size; j++)
					data[j*size+i] = 0.0f;

			// random offset bounds
			float h = 0.1f;

			// Generate height values by Square Diamond algorithm (based on Daniel Beard's implementation)
			for (int sideLength = size-1; sideLength >= 2; sideLength /= 2, h *= 0.5f)	{
		
				int halfSide = sideLength/2;

				//generate new square values
				for (int x = 0; x < size-1; x += sideLength) {
					for (int y = 0; y < size-1; y += sideLength) {

						// Calculate average of 4 corners
						float avg = (data[y*size + x] +
							data[y*size + x+sideLength]	+
							data[(y+sideLength)*size + x] +
							data[(y+sideLength)*size + x+sideLength]) / 4;

						// Set center location as average + random offset
						float offset = float(r.rand11() * h);
						data[(y+halfSide)*size + x+halfSide] = avg + offset;

					}
				}

				// Generate the diamond values
				for (int x = 0; x < size-1; x += halfSide) {
					for (int y = (x+halfSide) % sideLength; y < size-1; y += sideLength) {

						// Calculate average for diamond
						float avg = (data[y*size + (x-halfSide+size) % size] +
							data[y*size + (x+halfSide) % size] +
							data[((y+halfSide) % size)*size + x] +
							data[((y-halfSide+size) % size)*size + x]) / 4;

						// random offset for average value
						float offset = float(r.rand11() * h);
						float value = avg + offset;
			
						// Set value for diamond center
						data[y*size + x] = value;

						// Wrap around
						if (x == 0)
							data[y*size + size-1] = value;
						if (y == 0)
							data[(size-1)*size + x] = value;
					}
				}
			}

			// Generate vertices and indices based on height map
			for (int j = 0; j < size; j++)
			{
				for (int i = 0; i < size; i++)
				{
					// calculate vertex coordinates
					float x = i*dx;
					float z = j*dz;
					float y = data[j*size + i];

					Mesh::vertex v = { glm::vec3(x, y, z), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(x, z) * textureSize, color };

					if (i < size-1 && j < size-1)
					{
						// Calculate normal
						glm::vec3 v1(x, y, z);
						glm::vec3 v2(i*dx, data[(j+1)*size + i], (j+1)*dz);
						glm::vec3 v3((i+1)*dx, data[j*size + (i+1)], j*dz);

						glm::vec3 n = glm::normalize(glm::cross(v2 - v1, v3 - v1));
						v.normal = n;

						// set indices for CCW winding order
						iData.push_back(j*size + i);
						iData.push_back((j+1)*size + i);
						iData.push_back(j*size + (i+1));

						iData.push_back(j*size + (i+1));
						iData.push_back((j+1)*size + i);
						iData.push_back((j+1)*size + (i+1));
					}

					vData.push_back(v);
				}
			}

			// create mesh
			Mesh * mesh = new Mesh(g_useStaticDraw);
			mesh->setVertices(vData);
			mesh->setIndices(iData);
			return mesh;
		}
	};	
} /* namespace Common */
