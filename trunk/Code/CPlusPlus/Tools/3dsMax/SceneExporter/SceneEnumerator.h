#pragma once

#include <string>

// Max includes
#include <Max.h>

#include "../../../Shared/FileIO/XML/XMLDocumentBuilder.h"

class SceneEnumerator : public ITreeEnumProc
{
public:
	SceneEnumerator(void);
	~SceneEnumerator(void);

	virtual int callback( INode* node );
	const XMLDocumentBuilder& GetDocumentBuilder() const;
private:
	XMLDocumentBuilder documentBuilder_;
};

