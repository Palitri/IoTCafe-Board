#ifndef ICommandProcessor_h
#define ICommandProcessor_h

/// <summary>
/// Represents an object, which can execute actions, defined by a stream of bytes
/// </summary>
class ICommandProcessor
{
public:
	/// @brief Processes a command with optional input data and returns true if the command was successfully processed.
	/// @param command Comand identifier.
	/// @param data Optional data for the command.
	/// @param dataSize Size in bytes of the command data.
	/// @return Returns true if the given command was successfully processed. Returns false if the given command is not recognized or was not processed successfully
	virtual bool ProcessCommand(unsigned char command, void* data, int dataSize) = 0;
};

#endif
// ICommandProcessor_h
