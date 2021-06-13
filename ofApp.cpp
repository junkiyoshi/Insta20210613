#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 180, true, true, true);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);

	vector<string> word_list = {
		"123",
		"456",
		"789"
	};

	vector<glm::vec2> base_location_list = {
		glm::vec2(this->font.stringWidth(word_list[0]) * 0.5 - ofGetWidth() * 0.5, -150),
		glm::vec2(this->font.stringWidth(word_list[1]) * 0.5 - ofGetWidth() * 0.5, 60),
		glm::vec2(this->font.stringWidth(word_list[2]) * 0.5 - ofGetWidth() * 0.5, 270)
	};

	for (int i = 0; i < word_list.size(); i++) {

		auto word = word_list[i];
		auto base_location = base_location_list[i];

		int sample_count = 120;
		vector<ofPath> word_path = this->font.getStringAsPoints(word, true, false);
		for (int word_index = 0; word_index < word_path.size(); word_index++) {

			vector<ofPolyline> outline = word_path[word_index].getOutline();
			for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

				outline[outline_index] = outline[outline_index].getResampledByCount(sample_count);
				vector<glm::vec3> vertices = outline[outline_index].getVertices();
				vector<glm::vec2> black_vertices, white_vertices;
				glm::vec2 circle_location;
				int param = ofGetFrameNum() + i * 10 + word_index * 10;

				for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

					auto location = base_location + vertices[vertices_index];

					if (param % (sample_count * 2) < sample_count) {

						if (vertices_index < param % sample_count) {

							black_vertices.push_back(location);
						}
						else {

							white_vertices.push_back(location);
						}
					}
					else {

						if (vertices_index > param % sample_count) {

							black_vertices.push_back(location);
						}
						else {

							white_vertices.push_back(location);
						}
					}

					if (vertices_index == param % sample_count) {

						circle_location = location;
					}

				}


				ofMesh face, line, line_2;
				line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
				line_2.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
				for (int k = 0; k < black_vertices.size(); k++) {

					face.addVertex(glm::vec3(black_vertices[k], -15));
					face.addVertex(glm::vec3(black_vertices[k], 15));

					line.addVertex(glm::vec3(black_vertices[k], -15));
					line.addVertex(glm::vec3(black_vertices[k], 15));

					if (k > 0) {

						face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 2); face.addIndex(face.getNumVertices() - 4);
						face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 3); face.addIndex(face.getNumVertices() - 4);

						line.addIndex(line.getNumVertices() - 1); line.addIndex(line.getNumVertices() - 3);
						line.addIndex(line.getNumVertices() - 2); line.addIndex(line.getNumVertices() - 4);
					}
				}

				line.addIndex(0); line.addIndex(1);
				line.addIndex(line.getNumVertices() - 1); line.addIndex(line.getNumVertices() - 2);


				for (int k = 0; k < white_vertices.size(); k++)	{

					line_2.addVertex(glm::vec3(white_vertices[k], -15));
					line_2.addVertex(glm::vec3(white_vertices[k], 15));

					if (k > 0) {

						line_2.addIndex(line_2.getNumVertices() - 1); line_2.addIndex(line_2.getNumVertices() - 3);
						line_2.addIndex(line_2.getNumVertices() - 2); line_2.addIndex(line_2.getNumVertices() - 4);
					}
				}

				line_2.addIndex(0); line_2.addIndex(1);
				line_2.addIndex(line_2.getNumVertices() - 1); line_2.addIndex(line_2.getNumVertices() - 2);

				ofSetColor(39);
				face.draw();

				ofSetColor(239, 39, 39);
				line.drawWireframe();
				//line_2.drawWireframe();
			}
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}