/////////  begin  1b679b70-2a62-11de-8c30-0800200c9a66  ///////////////////////
// Event
// Assertions=true
// InstanceCount=false
// NumberOfArguments=0
///////////////////////////////////////////////////////////////////////////////
template < typename ReturnType >
class Event0
{
public:
	ConnectionHandle Connect(Callback0< ReturnType > Callback)
	{
		return ConnectionHandle(_Callbacks.Append(Callback));
	}
	
	void Disconnect(ConnectionHandle ConnectionHandle)
	{
		_Callbacks.Remove(static_cast< Node< Callback0< ReturnType > > * >(ConnectionHandle.GetHandle()));
		ConnectionHandle.Invalidate();
	}
	
	void operator()(void)
	{
		for(Node< Callback0< ReturnType > > * CallbackNode = _Callbacks.GetHead(); CallbackNode != 0; CallbackNode = CallbackNode->GetNext())
		{
			CallbackNode->GetValue()();
		}
	}
private:
	List< Callback0< ReturnType > > _Callbacks;
};
/////////  end    1b679b70-2a62-11de-8c30-0800200c9a66  ///////////////////////

