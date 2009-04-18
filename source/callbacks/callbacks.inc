/////////  begin  e7be1f35-8091-47aa-b300-b94abd95708e  ///////////////////////
// Counter
// Assertions=false
// InstanceCount=false
///////////////////////////////////////////////////////////////////////////////
class Counter
{
public:
	Counter(void) :
		m_Count(0)
	{
	}
	
	void Increment(void)
	{
		m_Count += 1;
	}
	
	void Decrement(void)
	{
		m_Count -= 1;
	}
	
	unsigned long int GetCount(void) const
	{
		return m_Count;
	}
private:
	unsigned long int m_Count;
};
/////////  end    e7be1f35-8091-47aa-b300-b94abd95708e  ///////////////////////

/////////  begin  5afb7876-076b-4ec6-9531-60f51d795b63  ///////////////////////
// Callback Core
// InstanceCount=false
// NumberOfArguments=0
///////////////////////////////////////////////////////////////////////////////
template < typename ReturnType >
class CallbackCore0 : public Counter
{
public:
	virtual ReturnType Call(void) const = 0;
	
	virtual bool IsValid(void) const = 0;
};
/////////  end    5afb7876-076b-4ec6-9531-60f51d795b63  ///////////////////////

/////////  begin  5afb7876-076b-4ec6-9531-60f51d795b63  ///////////////////////
// Callback Core
// InstanceCount=false
// NumberOfArguments=1
///////////////////////////////////////////////////////////////////////////////
template < typename ReturnType, typename Argument1Type >
class CallbackCore1 : public Counter
{
public:
	virtual ReturnType Call(Argument1Type Argument1) const = 0;
	
	virtual bool IsValid(void) const = 0;
};
/////////  end    5afb7876-076b-4ec6-9531-60f51d795b63  ///////////////////////

/////////  begin  5afb7876-076b-4ec6-9531-60f51d795b63  ///////////////////////
// Callback Core
// InstanceCount=false
// NumberOfArguments=2
///////////////////////////////////////////////////////////////////////////////
template < typename ReturnType, typename Argument1Type, typename Argument2Type >
class CallbackCore2 : public Counter
{
public:
	virtual ReturnType Call(Argument1Type Argument1, Argument2Type Argument2) const = 0;
	
	virtual bool IsValid(void) const = 0;
};
/////////  end    5afb7876-076b-4ec6-9531-60f51d795b63  ///////////////////////

/////////  begin  ccf8225b-77a6-4f78-ae69-ffd3060466bf  ///////////////////////
// Function Core
// Assertions=false
// InstanceCount=false
// NumberOfArguments=0
///////////////////////////////////////////////////////////////////////////////
template < typename ReturnType >
class FunctionCore0 : public CallbackCore0< ReturnType >
{
public:
	typedef ReturnType (*Function)();
	
	explicit FunctionCore0(Function Function) :
		m_Function(Function)
	{
	}
	
	virtual ReturnType Call(void) const
	{
		return (*m_Function)();
	}
	
	virtual bool IsValid(void) const
	{
		return m_Function != 0;
	}
protected:
	Function m_Function;
};
/////////  end    ccf8225b-77a6-4f78-ae69-ffd3060466bf  ///////////////////////

/////////  begin  ccf8225b-77a6-4f78-ae69-ffd3060466bf  ///////////////////////
// Function Core
// Assertions=false
// InstanceCount=false
// NumberOfArguments=1
///////////////////////////////////////////////////////////////////////////////
template < typename ReturnType, typename Argument1Type >
class FunctionCore1 : public CallbackCore1< ReturnType, Argument1Type >
{
public:
	typedef ReturnType (*Function)(Argument1Type);
	
	explicit FunctionCore1(Function Function) :
		m_Function(Function)
	{
	}
	
	virtual ReturnType Call(Argument1Type Argument1) const
	{
		return (*m_Function)(Argument1);
	}
	
	virtual bool IsValid(void) const
	{
		return m_Function != 0;
	}
protected:
	Function m_Function;
};
/////////  end    ccf8225b-77a6-4f78-ae69-ffd3060466bf  ///////////////////////

/////////  begin  b383e6d4-5685-49d8-8289-ac19f517cc21  ///////////////////////
// Method Core
// Assertions=false
// InstanceCount=false
// NumberOfArguments=0
///////////////////////////////////////////////////////////////////////////////
template < typename Class, typename ReturnType >
class MethodCore0 : public CallbackCore0< ReturnType >
{
public:
	typedef ReturnType (Class::*Method)(void);
	
	explicit MethodCore0(Class * Object, Method Method) :
		m_Object(Object),
		m_Method(Method)
	{
	}
	
	virtual ReturnType Call(void) const
	{
		return (m_Object->*(this->m_Method))();
	}
	
	virtual bool IsValid(void) const
	{
		return (m_Object != 0) && (m_Method != 0);
	}
protected:
	Class * m_Object;
	Method m_Method;
};
/////////  end    b383e6d4-5685-49d8-8289-ac19f517cc21  ///////////////////////

/////////  begin  b383e6d4-5685-49d8-8289-ac19f517cc21  ///////////////////////
// Method Core
// Assertions=false
// InstanceCount=false
// NumberOfArguments=1
///////////////////////////////////////////////////////////////////////////////
template < typename Class, typename ReturnType, typename Argument1Type >
class MethodCore1 : public CallbackCore1< ReturnType, Argument1Type >
{
public:
	typedef ReturnType (Class::*Method)(Argument1Type Argument1);
	
	explicit MethodCore1(Class * Object, Method Method) :
		m_Object(Object),
		m_Method(Method)
	{
	}
	
	virtual ReturnType Call(Argument1Type Argument1) const
	{
		return (m_Object->*(this->m_Method))(Argument1);
	}
	
	virtual bool IsValid(void) const
	{
		return (m_Object != 0) && (m_Method != 0);
	}
protected:
	Class * m_Object;
	Method m_Method;
};
/////////  end    b383e6d4-5685-49d8-8289-ac19f517cc21  ///////////////////////

/////////  begin  b383e6d4-5685-49d8-8289-ac19f517cc21  ///////////////////////
// Method Core
// Assertions=false
// InstanceCount=false
// NumberOfArguments=2
///////////////////////////////////////////////////////////////////////////////
template < typename Class, typename ReturnType, typename Argument1Type, typename Argument2Type >
class MethodCore2 : public CallbackCore2< ReturnType, Argument1Type, Argument2Type >
{
public:
	typedef ReturnType (Class::*Method)(Argument1Type Argument1, Argument2Type Argument2);
	
	explicit MethodCore2(Class * Object, Method Method) :
		m_Object(Object),
		m_Method(Method)
	{
	}
	
	virtual ReturnType Call(Argument1Type Argument1, Argument2Type Argument2) const
	{
		return (m_Object->*(this->m_Method))(Argument1, Argument2);
	}
	
	virtual bool IsValid(void) const
	{
		return (m_Object != 0) && (m_Method != 0);
	}
protected:
	Class * m_Object;
	Method m_Method;
};
/////////  end    b383e6d4-5685-49d8-8289-ac19f517cc21  ///////////////////////

/////////  begin  8f5fd530-285b-43ce-bdab-a97185232a1e  ///////////////////////
// Callback
// Assertions=false
// InstanceCount=false
// NumberOfArguments=0
///////////////////////////////////////////////////////////////////////////////
/**
 * @brief A light wrapper around a CallbackCore0-derivative.
 * 
 * This wrapper can be copied and assigned and is therefore ideal for being handled by value. It can be passed by value and included in by-value-containers.
 **/
template < typename ReturnType >
class Callback0
{
public:
	Callback0(void) :
		m_Core(0)
	{
	}
	
	Callback0(CallbackCore0< ReturnType > * Core) :
		m_Core(Core)
	{
		m_Core->Increment();
	}
	
	Callback0(const Callback0 & OtherCallback) :
		m_Core(OtherCallback.m_Core)
	{
		m_Core->Increment();
	}
	
	~Callback0(void)
	{
		if(m_Core != 0)
		{
			m_Core->Decrement();
			if(m_Core->GetCount() == 0)
			{
				delete m_Core;
				m_Core = 0;
			}
		}
	}
	
	Callback0 & operator=(const Callback0 & Callback)
	{
		if(m_Core != 0)
		{
			m_Core->Decrement();
			if(m_Core->GetCount() == 0)
			{
				delete m_Core;
				m_Core = 0;
			}
		}
		m_Core = Callback.m_Core;
		if(m_Core != 0)
		{
			m_Core->Increment();
		}
		
		return *this;
	}
	
	ReturnType operator()(void) const
	{
		return m_Core->Call();
	}
	
	bool IsValid(void) const
	{
		return (m_Core != 0) && (m_Core->IsValid() == true);
	}
	
	CallbackCore0< ReturnType > * GetCore(void)
	{
		return m_Core;
	}
	
	const CallbackCore0< ReturnType > * GetCore(void) const
	{
		return m_Core;
	}
private:
	CallbackCore0< ReturnType > * m_Core;
};
/////////  end    8f5fd530-285b-43ce-bdab-a97185232a1e  ///////////////////////

/////////  begin  8f5fd530-285b-43ce-bdab-a97185232a1e  ///////////////////////
// Callback
// Assertions=false
// InstanceCount=false
// NumberOfArguments=1
///////////////////////////////////////////////////////////////////////////////
/**
 * @brief A light wrapper around a CallbackCore1-derivative.
 * 
 * This wrapper can be copied and assigned and is therefore ideal for being handled by value. It can be passed by value and included in by-value-containers.
 **/
template < typename ReturnType, typename Argument1Type >
class Callback1
{
public:
	Callback1(void) :
		m_Core(0)
	{
	}
	
	Callback1(CallbackCore1< ReturnType, Argument1Type > * Core) :
		m_Core(Core)
	{
		m_Core->Increment();
	}
	
	Callback1(const Callback1 & OtherCallback) :
		m_Core(OtherCallback.m_Core)
	{
		m_Core->Increment();
	}
	
	~Callback1(void)
	{
		if(m_Core != 0)
		{
			m_Core->Decrement();
			if(m_Core->GetCount() == 0)
			{
				delete m_Core;
				m_Core = 0;
			}
		}
	}
	
	Callback1 & operator=(const Callback1 & Callback)
	{
		if(m_Core != 0)
		{
			m_Core->Decrement();
			if(m_Core->GetCount() == 0)
			{
				delete m_Core;
				m_Core = 0;
			}
		}
		m_Core = Callback.m_Core;
		if(m_Core != 0)
		{
			m_Core->Increment();
		}
		
		return *this;
	}
	
	ReturnType operator()(Argument1Type Argument1) const
	{
		return m_Core->Call(Argument1);
	}
	
	bool IsValid(void) const
	{
		return (m_Core != 0) && (m_Core->IsValid() == true);
	}
	
	CallbackCore1< ReturnType, Argument1Type > * GetCore(void)
	{
		return m_Core;
	}
	
	const CallbackCore1< ReturnType, Argument1Type > * GetCore(void) const
	{
		return m_Core;
	}
private:
	CallbackCore1< ReturnType, Argument1Type > * m_Core;
};
/////////  end    8f5fd530-285b-43ce-bdab-a97185232a1e  ///////////////////////

/////////  begin  8f5fd530-285b-43ce-bdab-a97185232a1e  ///////////////////////
// Callback
// Assertions=false
// InstanceCount=false
// NumberOfArguments=2
///////////////////////////////////////////////////////////////////////////////
/**
 * @brief A light wrapper around a CallbackCore2-derivative.
 * 
 * This wrapper can be copied and assigned and is therefore ideal for being handled by value. It can be passed by value and included in by-value-containers.
 **/
template < typename ReturnType, typename Argument1Type, typename Argument2Type >
class Callback2
{
public:
	Callback2(void) :
		m_Core(0)
	{
	}
	
	Callback2(CallbackCore2< ReturnType, Argument1Type, Argument2Type > * Core) :
		m_Core(Core)
	{
		m_Core->Increment();
	}
	
	Callback2(const Callback2 & OtherCallback) :
		m_Core(OtherCallback.m_Core)
	{
		m_Core->Increment();
	}
	
	~Callback2(void)
	{
		if(m_Core != 0)
		{
			m_Core->Decrement();
			if(m_Core->GetCount() == 0)
			{
				delete m_Core;
				m_Core = 0;
			}
		}
	}
	
	Callback2 & operator=(const Callback2 & Callback)
	{
		if(m_Core != 0)
		{
			m_Core->Decrement();
			if(m_Core->GetCount() == 0)
			{
				delete m_Core;
				m_Core = 0;
			}
		}
		m_Core = Callback.m_Core;
		if(m_Core != 0)
		{
			m_Core->Increment();
		}
		
		return *this;
	}
	
	ReturnType operator()(Argument1Type Argument1, Argument2Type Argument2) const
	{
		return m_Core->Call(Argument1, Argument2);
	}
	
	bool IsValid(void) const
	{
		return (m_Core != 0) && (m_Core->IsValid() == true);
	}
	
	CallbackCore2< ReturnType, Argument1Type, Argument2Type > * GetCore(void)
	{
		return m_Core;
	}
	
	const CallbackCore2< ReturnType, Argument1Type, Argument2Type > * GetCore(void) const
	{
		return m_Core;
	}
private:
	CallbackCore2< ReturnType, Argument1Type, Argument2Type > * m_Core;
};
/////////  end    8f5fd530-285b-43ce-bdab-a97185232a1e  ///////////////////////

/////////  begin  9730669a-993e-4d2a-9e3d-25af128e728f  ///////////////////////
// Bind Argument Core
// Assertions=false
// BindArgument=1
// InstanceCount=false
// NumberOfArguments=1
///////////////////////////////////////////////////////////////////////////////
template < typename ReturnType, typename Argument1Type >
class BindArgument1Core0 : public CallbackCore0< ReturnType >
{
public:
	explicit BindArgument1Core0(Callback1< ReturnType, Argument1Type > Callback, Argument1Type Argument1) :
		m_CallbackCore(Callback.GetCore()),
		m_Argument1(Argument1)
	{
		m_CallbackCore->Increment();
		
	}
	
	virtual ~BindArgument1Core0(void)
	{
		if(m_CallbackCore != 0)
		{
			m_CallbackCore->Decrement();
			if(m_CallbackCore->GetCount() == 0)
			{
				delete m_CallbackCore;
				m_CallbackCore = 0;
			}
		}
	}
	
	virtual ReturnType Call() const
	{
		return m_CallbackCore->Call(m_Argument1);
	}
	
	virtual bool IsValid(void) const
	{
		return m_CallbackCore->IsValid();
	}
protected:
	CallbackCore1< ReturnType, Argument1Type > * m_CallbackCore;
	Argument1Type m_Argument1;
};
/////////  end    9730669a-993e-4d2a-9e3d-25af128e728f  ///////////////////////

/////////  begin  a0f7a88b-e6d2-470d-a371-1d21950dea90  ///////////////////////
// Function Callback
// NumberOfArguments=0
///////////////////////////////////////////////////////////////////////////////
template < typename ReturnType >
Callback0< ReturnType > Callback(ReturnType (*Function)())
{
	return Callback0< ReturnType >(new FunctionCore0< ReturnType >(Function));
}
/////////  end    a0f7a88b-e6d2-470d-a371-1d21950dea90  ///////////////////////

/////////  begin  a0f7a88b-e6d2-470d-a371-1d21950dea90  ///////////////////////
// Function Callback
// NumberOfArguments=1
///////////////////////////////////////////////////////////////////////////////
template < typename ReturnType, typename Argument1Type >
Callback1< ReturnType, Argument1Type > Callback(ReturnType (*Function)(Argument1Type))
{
	return Callback1< ReturnType, Argument1Type >(new FunctionCore1< ReturnType, Argument1Type >(Function));
}
/////////  end    a0f7a88b-e6d2-470d-a371-1d21950dea90  ///////////////////////

/////////  begin  89e43761-647f-4c24-88d4-76caa736d627  ///////////////////////
// Method Callback
// NumberOfArguments=0
///////////////////////////////////////////////////////////////////////////////
template < typename ClassType, typename ReturnType >
Callback0< ReturnType > Callback(ClassType * Object, ReturnType (ClassType::*Method)())
{
	return Callback0< ReturnType >(new MethodCore0< ClassType, ReturnType >(Object, Method));
}
/////////  end    89e43761-647f-4c24-88d4-76caa736d627  ///////////////////////

/////////  begin  89e43761-647f-4c24-88d4-76caa736d627  ///////////////////////
// Method Callback
// NumberOfArguments=1
///////////////////////////////////////////////////////////////////////////////
template < typename ClassType, typename ReturnType, typename Argument1Type >
Callback1< ReturnType, Argument1Type > Callback(ClassType * Object, ReturnType (ClassType::*Method)(Argument1Type))
{
	return Callback1< ReturnType, Argument1Type >(new MethodCore1< ClassType, ReturnType, Argument1Type >(Object, Method));
}
/////////  end    89e43761-647f-4c24-88d4-76caa736d627  ///////////////////////

/////////  begin  89e43761-647f-4c24-88d4-76caa736d627  ///////////////////////
// Method Callback
// NumberOfArguments=2
///////////////////////////////////////////////////////////////////////////////
template < typename ClassType, typename ReturnType, typename Argument1Type, typename Argument2Type >
Callback2< ReturnType, Argument1Type, Argument2Type > Callback(ClassType * Object, ReturnType (ClassType::*Method)(Argument1Type, Argument2Type))
{
	return Callback2< ReturnType, Argument1Type, Argument2Type >(new MethodCore2< ClassType, ReturnType, Argument1Type, Argument2Type >(Object, Method));
}
/////////  end    89e43761-647f-4c24-88d4-76caa736d627  ///////////////////////

/////////  begin  15c882f3-3fea-466b-8347-4074787f0f5b  ///////////////////////
// Bind
// BindArgument=1
// NumberOfArguments=1
///////////////////////////////////////////////////////////////////////////////
template < typename ReturnType, typename Argument1Type >
Callback0< ReturnType > Bind1(Callback1< ReturnType, Argument1Type > Callback, Argument1Type Argument1)
{
	return Callback0< ReturnType >(new BindArgument1Core0< ReturnType, Argument1Type >(Callback, Argument1));
}
/////////  end    15c882f3-3fea-466b-8347-4074787f0f5b  ///////////////////////

