#include "Writer.h"
#include <unordered_map>
#include <utility>
#include <cassert>
using namespace std;

// hash function for pair<int, int>
namespace std {
    template <>
    struct hash<std::pair<int, int>> {
        size_t operator()(const std::pair<int, int>& k) const {
            return ((hash<int>()(k.first) ^ (hash<int>()(k.second) << 1)) >> 1);
        }
    };
}

void Writer::split_vertices_by_label(Model& model) {
	int vs = model.vertices.size();
	int sz = model.faces.size();
	// (old vid, label) -> new vid
	unordered_map<pair<int, int>, int> vid_map;
	vector<Vertex> new_vertices;
	// vector<Face> new_faces;
	for (int i = 0; i < sz; ++i) {
		Face& f = model.faces[i];
		for (int j = 0; j < 3; ++j) {
			int vid = f.vids[j];
			int label = f.label;
			if (vid_map.find(make_pair(vid, label)) == vid_map.end()) {
				vid_map[make_pair(vid, label)] = new_vertices.size();
				new_vertices.push_back(model.vertices[vid]);
			}
			f.vids[j] = vid_map[make_pair(vid, label)];
		}
	}
	model.vertices = new_vertices;
}

void Writer::write(string fileName, Model& model, bool per_face_corner_color)
{
	if (per_face_corner_color) {
		int vs = model.vertices.size();
		int sz = model.faces.size();
		ofstream fout(fileName);
		fout << "ply" << endl;
		fout << "format ascii 1.0" << endl;
		fout << "element vertex " << vs << endl;
		fout << "property float x" << endl;
		fout << "property float y" << endl;
		fout << "property float z" << endl;
		fout << "element face " << sz << endl;
		fout << "property list uchar int vertex_indices" << endl;
		fout << "property uint8 red" << endl;
		fout << "property uint8 green" << endl;
		fout << "property uint8 blue" << endl;
		fout << "end_header" << endl;
		for (int i = 0; i < vs; ++i)
		{
			for (int j = 0; j < 3; ++j)
				fout << model.vertices[i].p.x[j] << " ";
			fout << endl;
		}
		for (int i = 0; i < sz; ++i)
		{
			fout << "3 ";
			for (int j = 0; j < 3; ++j)
				fout << model.faces[i].vids[j] << " ";
			int label = model.faces[i].label;
			fout << 60 * (label % 4 + 1) << " " << 80 * ((label + 1) % 3 + 1) << " " << 50 * ((label + 2) % 5 + 1) << endl;
		}
		fout.close();
	} else {
		split_vertices_by_label(model);
		// vid to label
		unordered_map<int, int> vid_to_label;
		for (int i = 0; i < model.faces.size(); ++i) {
			Face& f = model.faces[i];
			for (int j = 0; j < 3; ++j) {
				if(vid_to_label.find(f.vids[j]) != vid_to_label.end()) {
					assert(vid_to_label[f.vids[j]] == f.label);
				}
				vid_to_label[f.vids[j]] = f.label;
			}
		}

		int vs = model.vertices.size();
		int sz = model.faces.size();
		ofstream fout(fileName);
		fout << "ply" << endl;
		fout << "format ascii 1.0" << endl;
		fout << "element vertex " << vs << endl;
		fout << "property float x" << endl;
		fout << "property float y" << endl;
		fout << "property float z" << endl;
		fout << "property uint8 red" << endl;
		fout << "property uint8 green" << endl;
		fout << "property uint8 blue" << endl;
		fout << "element face " << sz << endl;
		fout << "property list uchar int vertex_indices" << endl;
		fout << "end_header" << endl;
		for (int i = 0; i < vs; ++i)
		{
			for (int j = 0; j < 3; ++j)
				fout << model.vertices[i].p.x[j] << " ";
			int label = vid_to_label[i];
			fout << 60 * (label % 4 + 1) << " " << 80 * ((label + 1) % 3 + 1) << " " << 50 * ((label + 2) % 5 + 1);
			fout << endl;
		}
		for (int i = 0; i < sz; ++i)
		{
			fout << "3 ";
			for (int j = 0; j < 3; ++j)
				fout << model.faces[i].vids[j] << " ";
			fout << endl;
		}
		fout.close();
	}
}