#pragma once

#include <raindance/Core/Headers.hh>

enum VariableType
{
	RD_UNDEFINED,
	RD_STRING,
	RD_INT,
	RD_FLOAT,
	RD_BOOLEAN,
	RD_VEC2,
	RD_VEC3,
	RD_VEC4
};

class IVariable
{
public:
    IVariable() : m_Type(RD_UNDEFINED) {}
	virtual ~IVariable() {}

	inline VariableType type() const { return m_Type; }
	inline void type(VariableType type) { m_Type = type; }

	inline const std::string& name() const { return m_Name; }
	inline void name(const std::string& name) { m_Name = name; }

	virtual void set(const std::string& str) = 0;
	virtual IVariable* duplicate() = 0;
protected:
	VariableType m_Type;
	std::string m_Name;
};

class StringVariable : public IVariable
{
public:
	inline virtual void set(const std::string& str) { m_Type = RD_STRING; m_Value = str; }
	inline const std::string& value() { return m_Value; }
	virtual IVariable* duplicate()
	{
	    StringVariable* variable = new StringVariable();
	    variable->name(m_Name);
	    variable->set(m_Value);
	    return variable;
	}
private:
	std::string m_Value;
};

class IntVariable : public IVariable
{
public:
	inline virtual void set(const std::string& str) { m_Type = RD_INT; m_Value = atoi(str.c_str()); }
   //inline virtual void set(int value) { m_Type = RD_INT; m_Value = value; }
    inline virtual void set(long value) { m_Type = RD_INT; m_Value = value; }
	inline long value() { return m_Value; }
	virtual IVariable* duplicate()
	{
        IntVariable* variable = new IntVariable();
        variable->name(m_Name);
        variable->set(m_Value);
        return variable;
	}
private:
	long m_Value;
};

class BooleanVariable : public IVariable
{
public:
	inline virtual void set(const std::string& str)
	{
		m_Type = RD_BOOLEAN;
		if (str == "1" || str == "true" || str == "True")
			m_Value = true;
		else
			m_Value = false;
	}
	inline virtual void set(bool value) { m_Type = RD_BOOLEAN; m_Value = value; }
	inline bool value() { return m_Value; }
	virtual BooleanVariable* duplicate()
	{
	    BooleanVariable* variable = new BooleanVariable();
	    variable->name(m_Name);
	    variable->set(m_Value);
	    return variable;
	}
private:
	bool m_Value;
};

class FloatVariable : public IVariable
{
public:
	inline virtual void set(const std::string& str) { m_Type = RD_FLOAT; m_Value = static_cast<float>(atof(str.c_str())); }
	inline virtual void set(float value) { m_Type = RD_FLOAT; m_Value = value; }
	inline float value() { return m_Value; }
	virtual IVariable* duplicate()
	{
	    FloatVariable* variable = new FloatVariable();
	    variable->name(m_Name);
	    variable->set(m_Value);
	    return variable;
	}
private:
	float m_Value;
};

class Vec2Variable : public IVariable
{
public:
	inline virtual void set(const std::string& str)
	{
		m_Type = RD_VEC2;
		std::stringstream ss(str);
		ss >> m_Value.x >> m_Value.y;
	}
	inline virtual void set(glm::vec2 value) { m_Type = RD_VEC2; m_Value = value; }
	inline const glm::vec2& value() { return m_Value; }
	virtual IVariable* duplicate()
	{
	    Vec2Variable* variable = new Vec2Variable();
	    variable->name(m_Name);
	    variable->set(m_Value);
	    return variable;
	}
private:
	glm::vec2 m_Value;
};

class Vec3Variable : public IVariable
{
public:
    inline virtual void set(const std::string& str)
    {
        m_Type = RD_VEC3;
        std::stringstream ss(str);
        ss >> m_Value.x >> m_Value.y >> m_Value.z;
    }
    inline virtual void set(glm::vec3 value) { m_Type = RD_VEC3; m_Value = value; }
    inline const glm::vec3& value() { return m_Value; }
    virtual IVariable* duplicate()
    {
        Vec3Variable* variable = new Vec3Variable();
        variable->name(m_Name);
        variable->set(m_Value);
        return variable;
    }
private:
    glm::vec3 m_Value;
};

class Vec4Variable : public IVariable
{
public:
    inline virtual void set(const std::string& str)
    {
        m_Type = RD_VEC4;
        std::stringstream ss(str);
        ss >> m_Value.x >> m_Value.y >> m_Value.z >> m_Value.w;
    }
    inline virtual void set(glm::vec4 value) { m_Type = RD_VEC4; m_Value = value; }
    inline const glm::vec4& value() { return m_Value; }
    virtual IVariable* duplicate()
    {
        Vec4Variable* variable = new Vec4Variable();
        variable->name(m_Name);
        variable->set(m_Value);
        return variable;
    }
private:
    glm::vec4 m_Value;
};

class Variables
{
public:
    Variables()
    {
    }

    virtual ~Variables()
    {
        /* TODO : Deleting the variables here leads to crashes ! Why ?
        while(!m_Variables.empty())
        {
            delete m_Variables.back();
            m_Variables.pop_back();
        }
        */
    }

    const IVariable* add(IVariable* var)
    {
        std::string name = var->name();

        std::vector<IVariable*>::iterator it;
        for (it = m_Variables.begin(); it != m_Variables.end(); ++it)
        {
            if (name == (*it)->name())
            {
                LOG("[VARIABLES] Variable name '%s' already exists!\n", name.c_str());
                return NULL;
            }
        }

        IVariable* newVar = var->duplicate();
        m_Variables.push_back(newVar);

        return newVar;
    }

	const IVariable* set(const std::string& name, VariableType type, const std::string& value)
	{
		// TODO : All of this can be optimized with a hash table

		std::vector<IVariable*>::iterator it;
		for (it = m_Variables.begin(); it != m_Variables.end(); ++it)
		{
			if (name == (*it)->name())
			{
				if (type != (*it)->type())
				{
					LOG("[VARIABLES] Variable type mismatches !\n");
					return NULL;
				}
				else
				{
					(*it)->set(value);
					return *it;
				}
			}
		}

		IVariable* variable = NULL;
		switch(type)
		{
		case RD_STRING:
			variable = new StringVariable();
			break;
		case RD_INT:
			variable = new IntVariable();
			break;
		case RD_FLOAT:
			variable = new FloatVariable();
			break;
		case RD_BOOLEAN:
			variable = new BooleanVariable();
			break;
		case RD_VEC2:
		    variable = new Vec2Variable();
		    break;
		case RD_VEC3:
            variable = new Vec3Variable();
            break;
        case RD_VEC4:
            variable = new Vec4Variable();
            break;
		default:
			LOG("[VARIABLES] Unknown variable type !\n");
			return NULL;
		}
		variable->name(name);
		variable->set(value);
		m_Variables.push_back(variable);

		return variable;
	}

	const IVariable* set(const char* name, VariableType type, std::string& value)
	{
	    return this->set(std::string(name), type, value);
	}

	IVariable* get(const std::string& name) const
	{
		std::vector<IVariable*>::const_iterator it;
		for (it = m_Variables.begin(); it != m_Variables.end(); ++it)
		{
			if (name == (*it)->name())
				return *it;
		}
		return NULL;
	}

	IVariable* get(const char* name) const
	{
		return this->get(std::string(name));
	}

    bool getString(const char* name, std::string& variable) const
    {
        IVariable* var = get(name);
        if (var == NULL || var->type() != RD_STRING)
            return false;
        variable.assign(static_cast<StringVariable*>(var)->value());
        return true;
    }

    bool getInt(const char* name, long* variable) const
    {
        IVariable* var = get(name);
        if (var == NULL || var->type() != RD_INT)
            return false;
        *variable = static_cast<IntVariable*>(var)->value();
        return true;
    }

    bool getFloat(const char* name, float* variable) const
    {
        IVariable* var = get(name);
        if (var == NULL || var->type() != RD_FLOAT)
            return false;
        *variable = static_cast<FloatVariable*>(var)->value();
        return true;
    }

    bool getBoolean(const char* name, bool* variable) const
    {
        IVariable* var = get(name);
        if (var == NULL || var->type() != RD_BOOLEAN)
            return false;
        *variable = static_cast<BooleanVariable*>(var)->value();
        return true;
    }

    bool getVec2(const char* name, glm::vec2* variable) const
    {
        IVariable* var = get(name);
        if (var == NULL || var->type() != RD_VEC2)
            return false;
        *variable = static_cast<Vec2Variable*>(var)->value();
        return true;
    }

    bool getVec3(const char* name, glm::vec3* variable) const
    {
        IVariable* var = get(name);
        if (var == NULL || var->type() != RD_VEC3)
            return false;
        *variable = static_cast<Vec3Variable*>(var)->value();
        return true;
    }

    bool getVec4(const char* name, glm::vec4* variable) const
    {
        IVariable* var = get(name);
        if (var == NULL || var->type() != RD_VEC4)
            return false;
        *variable = static_cast<Vec4Variable*>(var)->value();
        return true;
    }

	void dump() const
	{
	    glm::vec2 v2;
	    glm::vec3 v3;
	    glm::vec4 v4;

	    for (auto v : m_Variables)
	    {
	        switch(v->type())
	        {
	        case RD_STRING:
	            LOG("[VARIABLES] string %s = %s\n", v->name().c_str(), static_cast<StringVariable*>(v)->value().c_str());
	            break;
	        case RD_INT:
	            LOG("[VARIABLES] int %s = %lu\n", v->name().c_str(), static_cast<IntVariable*>(v)->value());
	            break;
	        case RD_FLOAT:
	            LOG("[VARIABLES] float %s = %f\n", v->name().c_str(), static_cast<FloatVariable*>(v)->value());
	            break;
	        case RD_BOOLEAN:
	            LOG("[VARIABLES] bool %s = %i\n", v->name().c_str(), static_cast<BooleanVariable*>(v)->value());
	            break;
	        case RD_VEC2:
	            v2 = static_cast<Vec2Variable*>(v)->value();
	            LOG("[VARIABLES] vec2 %s = (%f, %f)\n", v->name().c_str(), v2.x, v2.y);
	            break;
	        case RD_VEC3:
	            v3 = static_cast<Vec3Variable*>(v)->value();
	            LOG("[VARIABLES] vec3 %s = (%f, %f, %f)\n", v->name().c_str(), v3.x, v3.y, v3.z);
	            break;
	        case RD_VEC4:
	            v4 = static_cast<Vec4Variable*>(v)->value();
	            LOG("[VARIABLES] vec4 %s = (%f, %f, %f, %f)\n", v->name().c_str(), v4.x, v4.y, v4.z, v4.w);
	            break;
	        default:
	            LOG("[VARIABLES] Unknown type!\n");
	            break;
	        }
	    }
	}

	inline std::vector<IVariable*>::iterator begin() { return m_Variables.begin(); }
	inline std::vector<IVariable*>::iterator end() { return m_Variables.end(); }

private:
	std::vector<IVariable*> m_Variables;
};

