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
		CallbackIterator(Node< std::function< ReturnType (void) > > * CallbackNode) :
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
		Node< std::function< ReturnType (void) > > * _CallbackNode;
	};
	
	ConnectionHandle Connect(std::function< ReturnType (void) > Callback)
	{
		return ConnectionHandle(_Callbacks.Append(Callback));
	}
	
	void Disconnect(ConnectionHandle & ConnectionHandle)
	{
		_Callbacks.Remove(static_cast< Node< std::function< ReturnType (void) > > * >(ConnectionHandle.GetHandle()));
		ConnectionHandle.Invalidate();
	}
	
	CallbackIterator GetCallbackIterator(void)
	{
		return CallbackIterator(_Callbacks.GetHead());
	}
	
	void operator()(void)
	{
		std::list< std::function< ReturnType (void) > > Callbacks;
		
		for(Node< std::function< ReturnType (void) > > * CallbackNode = _Callbacks.GetHead(); CallbackNode != 0; CallbackNode = CallbackNode->GetNext())
		{
			Callbacks.push_back(CallbackNode->GetValue());
		}
		for(auto & Callback : Callbacks)
		{
			Callback();
		}
	}
private:
	List< std::function< ReturnType (void) > > _Callbacks;
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
		CallbackIterator(Node< std::function< ReturnType (Argument1Type) > > * CallbackNode) :
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
		Node< std::function< ReturnType (Argument1Type) > > * _CallbackNode;
	};
	
	ConnectionHandle Connect(std::function< ReturnType (Argument1Type) > Callback)
	{
		return ConnectionHandle(_Callbacks.Append(Callback));
	}
	
	void Disconnect(ConnectionHandle & ConnectionHandle)
	{
		_Callbacks.Remove(static_cast< Node< std::function< ReturnType (Argument1Type) > > * >(ConnectionHandle.GetHandle()));
		ConnectionHandle.Invalidate();
	}
	
	CallbackIterator GetCallbackIterator(void)
	{
		return CallbackIterator(_Callbacks.GetHead());
	}
	
	void operator()(Argument1Type Argument1)
	{
		std::list< std::function< ReturnType (Argument1Type) > > Callbacks;
		
		for(Node< std::function< ReturnType (Argument1Type) > > * CallbackNode = _Callbacks.GetHead(); CallbackNode != 0; CallbackNode = CallbackNode->GetNext())
		{
			Callbacks.push_back(CallbackNode->GetValue());
		}
		for(auto & Callback : Callbacks)
		{
			Callback(Argument1);
		}
	}
private:
	List< std::function< ReturnType (Argument1Type) > > _Callbacks;
};
/////////  end    1b679b70-2a62-11de-8c30-0800200c9a66  ///////////////////////

/////////  begin  1b679b70-2a62-11de-8c30-0800200c9a66  ///////////////////////
// Event
// Assertions=true
// InstanceCount=false
// NumberOfArguments=2
///////////////////////////////////////////////////////////////////////////////
template < typename ReturnType, typename Argument1Type, typename Argument2Type >
class Event2
{
public:
	class CallbackIterator
	{
	public:
		CallbackIterator(Node< std::function< ReturnType (Argument1Type, Argument2Type) > > * CallbackNode) :
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
		
		ReturnType operator()(Argument1Type Argument1, Argument2Type Argument2)
		{
			return _CallbackNode->GetValue()(Argument1, Argument2);
		}
	private:
		Node< std::function< ReturnType (Argument1Type, Argument2Type) > > * _CallbackNode;
	};
	
	ConnectionHandle Connect(std::function< ReturnType (Argument1Type, Argument2Type) > Callback)
	{
		return ConnectionHandle(_Callbacks.Append(Callback));
	}
	
	void Disconnect(ConnectionHandle & ConnectionHandle)
	{
		_Callbacks.Remove(static_cast< Node< std::function< ReturnType (Argument1Type, Argument2Type) > > * >(ConnectionHandle.GetHandle()));
		ConnectionHandle.Invalidate();
	}
	
	CallbackIterator GetCallbackIterator(void)
	{
		return CallbackIterator(_Callbacks.GetHead());
	}
	
	void operator()(Argument1Type Argument1, Argument2Type Argument2)
	{
		std::list< std::function< ReturnType (Argument1Type, Argument2Type) > > Callbacks;
		
		for(Node< std::function< ReturnType (Argument1Type, Argument2Type) > > * CallbackNode = _Callbacks.GetHead(); CallbackNode != 0; CallbackNode = CallbackNode->GetNext())
		{
			Callbacks.push_back(CallbackNode->GetValue());
		}
		for(auto & Callback : Callbacks)
		{
			Callback(Argument1, Argument2);
		}
	}
private:
	List< std::function< ReturnType (Argument1Type, Argument2Type) > > _Callbacks;
};
/////////  end    1b679b70-2a62-11de-8c30-0800200c9a66  ///////////////////////

/////////  begin  1b679b70-2a62-11de-8c30-0800200c9a66  ///////////////////////
// Event
// Assertions=true
// InstanceCount=false
// NumberOfArguments=4
///////////////////////////////////////////////////////////////////////////////
template < typename ReturnType, typename Argument1Type, typename Argument2Type, typename Argument3Type, typename Argument4Type >
class Event4
{
public:
	class CallbackIterator
	{
	public:
		CallbackIterator(Node< std::function< ReturnType (Argument1Type, Argument2Type, Argument3Type, Argument4Type) > > * CallbackNode) :
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
		
		ReturnType operator()(Argument1Type Argument1, Argument2Type Argument2, Argument3Type Argument3, Argument4Type Argument4)
		{
			return _CallbackNode->GetValue()(Argument1, Argument2, Argument3, Argument4);
		}
	private:
		Node< std::function< ReturnType (Argument1Type, Argument2Type, Argument3Type, Argument4Type) > > * _CallbackNode;
	};
	
	ConnectionHandle Connect(std::function< ReturnType (Argument1Type, Argument2Type, Argument3Type, Argument4Type) > Callback)
	{
		return ConnectionHandle(_Callbacks.Append(Callback));
	}
	
	void Disconnect(ConnectionHandle & ConnectionHandle)
	{
		_Callbacks.Remove(static_cast< Node< std::function< ReturnType (Argument1Type, Argument2Type, Argument3Type, Argument4Type) > > * >(ConnectionHandle.GetHandle()));
		ConnectionHandle.Invalidate();
	}
	
	CallbackIterator GetCallbackIterator(void)
	{
		return CallbackIterator(_Callbacks.GetHead());
	}
	
	void operator()(Argument1Type Argument1, Argument2Type Argument2, Argument3Type Argument3, Argument4Type Argument4)
	{
		std::list< std::function< ReturnType (Argument1Type, Argument2Type, Argument3Type, Argument4Type) > > Callbacks;
		
		for(Node< std::function< ReturnType (Argument1Type, Argument2Type, Argument3Type, Argument4Type) > > * CallbackNode = _Callbacks.GetHead(); CallbackNode != 0; CallbackNode = CallbackNode->GetNext())
		{
			Callbacks.push_back(CallbackNode->GetValue());
		}
		for(auto & Callback : Callbacks)
		{
			Callback(Argument1, Argument2, Argument3, Argument4);
		}
	}
private:
	List< std::function< ReturnType (Argument1Type, Argument2Type, Argument3Type, Argument4Type) > > _Callbacks;
};
/////////  end    1b679b70-2a62-11de-8c30-0800200c9a66  ///////////////////////

