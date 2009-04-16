class ConnectionHandle
{
public:
	ConnectionHandle(void * Handle) :
		_Handle(Handle)
	{
	}
	
	void * GetHandle(void)
	{
		return _Handle;
	}
private:
	void * _Handle;
};
