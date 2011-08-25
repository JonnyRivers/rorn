#pragma once

#include "D3DVertexShader.h"

using namespace Rorn::Engine;

D3DVertexShader::D3DVertexShader(ID3D11VertexShader* vertexShader, ID3D11InputLayout* vertexInput)
	: vertexShader_(vertexShader), vertexInput_(vertexInput)
{
}

D3DVertexShader::~D3DVertexShader()
{
	vertexInput_->Release();
	vertexShader_->Release();
}