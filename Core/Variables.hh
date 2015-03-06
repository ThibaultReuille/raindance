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

	virtual void set(const std::string& str) = 0;
	virtual IVariable* duplicate() = 0;

protected:
	VariableType m_Type;
	// std::string m_Name;
};

class StringVariable : public IVariable
{
public:
	inline virtual void set(const std::string& str) { m_Type = RD_STRING; m_Value = str; }
	inline const std::string& value() { return m_Value; }
	virtual IVariable* duplicate()
	{
	    StringVariable* variable = new StringVariable();
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
    inline virtual void set(long value) { m_Type = RD_INT; m_Value = value; }
	inline long value() { return m_Value; }
	virtual IVariable* duplicate()
	{
        IntVariable* variable = new IntVariable();
        // variable->name(m_Name);
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
        variable->set(m_Value);
        return variable;
    }
private:
    glm::vec4 m_Value;
};


class Variables
{
public:
	Variables() {}

	virtual ~Variables()
	{
	}

	virtual IVariable* set(const std::string& name, IVariable* variable)
	{
		if (m_Symbols.find(name) == m_Symbols.end())
			m_Symbols[name] = variable;
		else
		{
			delete m_Symbols[name];
			m_Symbols[name] = variable;
		}

		return variable;
	}

	virtual IVariable* set(const std::string& name, VariableType type, const std::string& value)
	{
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
		
		variable->set(value);
		
		return set(name, variable);
	}

	virtual IVariable* get(const std::string& name) const
	{
		auto element = m_Symbols.find(name);
		return (element != m_Symbols.end() ? element->second : NULL);
	}


	virtual void dump() const
	{
	    glm::vec2 v2;
	    glm::vec3 v3;
	    glm::vec4 v4;

	    for (auto it : m_Symbols)
	    {
	    	auto name = it.first;
	    	auto value = it.second;

	        switch(value->type())
	        {
	        case RD_STRING:
	            LOG("[VARIABLES] string %s = %s\n", name.c_str(), static_cast<StringVariable*>(value)->value().c_str());
	            break;
	        case RD_INT:
	            LOG("[VARIABLES] int %s = %lu\n", name.c_str(), static_cast<IntVariable*>(value)->value());
	            break;
	        case RD_FLOAT:
	            LOG("[VARIABLES] float %s = %f\n", name.c_str(), static_cast<FloatVariable*>(value)->value());
	            break;
	        case RD_BOOLEAN:
	            LOG("[VARIABLES] bool %s = %i\n", name.c_str(), static_cast<BooleanVariable*>(value)->value());
	            break;
	        case RD_VEC2:
	            v2 = static_cast<Vec2Variable*>(value)->value();
	            LOG("[VARIABLES] vec2 %s = (%f, %f)\n", name.c_str(), v2.x, v2.y);
	            break;
	        case RD_VEC3:
	            v3 = static_cast<Vec3Variable*>(value)->value();
	            LOG("[VARIABLES] vec3 %s = (%f, %f, %f)\n", name.c_str(), v3.x, v3.y, v3.z);
	            break;
	        case RD_VEC4:
	            v4 = static_cast<Vec4Variable*>(value)->value();
	            LOG("[VARIABLES] vec4 %s = (%f, %f, %f, %f)\n", name.c_str(), v4.x, v4.y, v4.z, v4.w);
	            break;
	        default:
	            LOG("[VARIABLES] Unknown type!\n");
	            break;
	        }
	    }
	}

	inline const std::unordered_map<std::string, IVariable*>& symbols() { return m_Symbols; }

	// ----- Specialized Getters -----

    bool get(const char* name, std::string& output) const
    {
        IVariable* var = get(name);
        if (var == NULL || var->type() != RD_STRING)
            return false;
        output.assign(static_cast<StringVariable*>(var)->value());
        return true;
    }

    bool get(const char* name, long* output) const
    {
        IVariable* var = get(name);
        if (var == NULL || var->type() != RD_INT)
            return false;
        *output = static_cast<IntVariable*>(var)->value();
        return true;
    }

    bool get(const char* name, float* output) const
    {
        IVariable* var = get(name);
        if (var == NULL || var->type() != RD_FLOAT)
            return false;
        *output = static_cast<FloatVariable*>(var)->value();
        return true;
    }

    bool get(const char* name, bool* output) const
    {
        IVariable* var = get(name);
        if (var == NULL || var->type() != RD_BOOLEAN)
            return false;
        *output = static_cast<BooleanVariable*>(var)->value();
        return true;
    }

    bool get(const char* name, glm::vec2* output) const
    {
        IVariable* var = get(name);
        if (var == NULL || var->type() != RD_VEC2)
            return false;
        *output = static_cast<Vec2Variable*>(var)->value();
        return true;
    }

    bool get(const char* name, glm::vec3* output) const
    {
        IVariable* var = get(name);
        if (var == NULL || var->type() != RD_VEC3)
            return false;
        *output = static_cast<Vec3Variable*>(var)->value();
        return true;
    }

    bool get(const char* name, glm::vec4* output) const
    {
        IVariable* var = get(name);
        if (var == NULL || var->type() != RD_VEC4)
            return false;
        *output = static_cast<Vec4Variable*>(var)->value();
        return true;
    }

private:
	std::unordered_map<std::string, IVariable*> m_Symbols;
};