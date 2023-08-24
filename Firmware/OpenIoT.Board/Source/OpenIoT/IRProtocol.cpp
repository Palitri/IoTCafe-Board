#include "IRProtocol.h"

#include "GenericIRProtocol.h"

#include "Board.h"

IRProtocol::IRProtocol()
{
}

IRProtocol::~IRProtocol()
{
}

IRProtocol* IRProtocol::Dispatch(void* data)
{
	unsigned char* charData = (unsigned char*)data;
	if (charData != null)
	{
		switch (*charData++)
		{
		case GenericIRProtocol::protocolId:
			return new GenericIRProtocol(charData);
		}
	}

	return new GenericIRProtocol(null);
}
