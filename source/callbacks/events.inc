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
	class CallbackIterator
	{
	public:
		CallbackIterator(Node< Callback0< ReturnType > > * CallbackNode) :
			_CallbackNode(CallbackNode)
		{
		}
		
		CallbackIterator(const CallbackIterator & CallbackIterator) :
			_CallbackNode(CallbackIterator._CallbackNode)
		{
		}
		
		CallbackIterator & operator=(const CallbackIterator & CallbackIterator)
		{
			_CallbackNode = CallbackIterator._CallbackNode;
			
			return *this;
		}
		
		CallbackIterator & operator++(void)
		{
			_CallbackNode = _CallbackNode->GetNext();
			
			return *this;
		}
		
		bool IsValid(void) const
		{
			return _CallbackNode != 0;
		}
		
		ReturnType operator()(void)
		{
			return _CallbackNode->GetValue()();
		}
	private:
		Node< Callback0< ReturnType > > * _CallbackNode;
	};
	
	ConnectionHandle Connect(Callback0< ReturnType > Callback)
	{
		return ConnectionHandle(_Callbacks.Append(Callback));
	}
	
	void Disconnect(ConnectionHandle & ConnectionHandle)
	{
		_Callbacks.Remove(static_cast< Node< Callback0< ReturnType > > * >(ConnectionHandle.GetHandle()));
		ConnectionHandle.Invalidate();
	}
	
	CallbackIterator GetCallbackIterator(void)
	{
		return CallbackIterator(_Callbacks.GetHead());
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

/////////  begin  1b679b70-2a62-11de-8c30-0800200c9a66  ///////////////////////
// Event
// Assertions=true
// InstanceCount=false
// NumberOfArguments=1
///////////////////////////////////////////////////////////////////////////////
template < typename ReturnType, typename Argument1Type >
class Event1
{
public:
	class CallbackIterator
	{
	public:
		CallbackIterator(Node< Callback1< ReturnType, Argument1Type > > * CallbackNode) :
			_CallbackNode(CallbackNode)
		{
		}
		
		CallbackIterator(const CallbackIterator & CallbackIterator) :
			_CallbackNode(CallbackIterator._CallbackNode)
		{
		}
		
		CallbackIterator & operator=(const CallbackIterator & CallbackIterator)
		{
			_CallbackNode = CallbackIterator._CallbackNode;
			
			return *this;
		}
		
		CallbackIterator & operator++(void)
		{
			_CallbackNode = _CallbackNode->GetNext();
			
			return *this;
		}
		
		bool IsValid(void) const
		{
			return _CallbackNode != 0;
		}
		
		ReturnType operator()(Argument1Type Argument1)
		{
			return _CallbackNode->GetValue()(Argument1);
		}
	private:
		Node< Callback1< ReturnType, Argument1Type > > * _CallbackNode;
	};
	
	ConnectionHandle Connect(Callback1< ReturnType, Argument1Type > Callback)
	{
		return ConnectionHandle(_Callbacks.Append(Callback));
	}
	
	void Disconnect(ConnectionHandle & ConnectionHandle)
	{
		_Callbacks.Remove(static_cast< Node< Callback1< ReturnType, Argument1Type > > * >(ConnectionHandle.GetHandle()));
		ConnectionHandle.Invalidate();
	}
	
	CallbackIterator GetCallbackIterator(void)
	{
		return CallbackIterator(_Callbacks.GetHead());
	}
	
	void operator()(Argument1Type Argument1)
	{
		for(Node< Callback1< ReturnType, Argument1Type > > * CallbackNode = _Callbacks.GetHead(); CallbackNode != 0; CallbackNode = CallbackNode->GetNext())
		{
			CallbackNode->GetValue()(Argument1);
		}
	}
private:
	List< Callback1< ReturnType, Argument1Type > > _Callbacks;
};
/////////  end    1b679b70-2a62-11de-8c30-0800200c9a66  ///////////////////////

