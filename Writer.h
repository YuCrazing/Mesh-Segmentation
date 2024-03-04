#pragma once
#include "Model.h"

class Writer
{
public:
	Writer() {}
	void split_vertices_by_label(Model& model);
	void write(string fileName, Model& model, bool per_face_corner_color=false);
};

