/*
    ###################################################################################
    PGEcfgVariable.cpp
    This file is part of PGE.
    PR00F's Game Engine cfg variable (cvar) class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PGESysCFG.h"
#include "PGEincludes.h"
#include "PGEpragmas.h"

using namespace std;


// ############################### PUBLIC ################################


/**
    Constructs a string-typed cvar.
*/
PGEcfgVariable::PGEcfgVariable()
{
    Set("");
}

/**
    Constructs an integer-typed cvar.
*/
PGEcfgVariable::PGEcfgVariable(const int& value)
{
    Set( value );
}

/**
    Constructs a floating point-typed cvar.
*/
PGEcfgVariable::PGEcfgVariable(const float& value)
{
    Set( value );
}

/**
    Constructs a boolean-typed cvar.
*/
PGEcfgVariable::PGEcfgVariable(const bool& value)
{
    Set( value );
}

/**
    Constructs a string-typed cvar.
*/
PGEcfgVariable::PGEcfgVariable(const char* value)
{
    Set( value );
}

PGEcfgVariable::~PGEcfgVariable()
{

}


/**
    Tries to return the value of the cvar as an integer.

    @return If the type of the cvar is bool or the cvar can be converted to bool, true value results in 1, and
            false value results in 0.
            If the type of the cvar is float or string, the result will be the same as calling atoi() with the float or string.
*/
int PGEcfgVariable::getAsInt() const
{
    if ( getType() == PGE_CVAR_BOOL )
        return getAsBool() ? 1 : 0;
    return atoi( sValue.c_str() );    
}

/**
    Tries to return the value of the cvar as a floating point number.

    @return If the type of the cvar is bool or the cvar can be converted to bool, true value results in 1, and
            false value results in 0.
            If the type of the cvar is string, the result will be the same as calling atof() with the string.
*/
float PGEcfgVariable::getAsFloat() const
{
    if ( getType() == PGE_CVAR_BOOL )
        return getAsBool() ? 1.0f : 0.0f;
    return (float)atof( sValue.c_str() );
}

/**
    Returns the value of the cvar as a boolean.

    @return True in the following cases:
             - type of cvar is string and value is "true" (not case-sensitive)
             - type of cvar is string and can be converted to a non-null numeric value
             - type of cvar is int or float and value is not 0
             - type of cvar is bool and value is true (trivial case)
            False in any other cases.
*/
bool PGEcfgVariable::getAsBool() const
{
    string tmpValue = sValue;
    transform(tmpValue.begin(), tmpValue.end(), tmpValue.begin(), ::toupper);

    if ( tmpValue == "TRUE" )
        return true;

    if ( tmpValue == "FALSE" )
        return false;

    return ( getAsInt() != 0 );
}

/**
    Returns the value of the cvar as a string.

    @return The value of the cvar as it is as a string.
*/
std::string PGEcfgVariable::getAsString() const
{
    return sValue;
}


/**
    Returns the type of the cvar.

    @return The type of the cvar.
            See detailed description at the description of TPGE_CFG_VARIABLE_TYPE.
*/
TPGE_CFG_VARIABLE_TYPE PGEcfgVariable::getType() const
{
    return type;
}


void PGEcfgVariable::Set(bool value)
{
    sValue = value ? "true" : "false";
    type = PGE_CVAR_BOOL;
}

void PGEcfgVariable::Set(float value)
{
    std::stringstream str;
    str << value;
    sValue = str.str();
    type = PGE_CVAR_FLOAT;
}

void PGEcfgVariable::Set(int value)
{
    std::stringstream str;
    str << value;
    sValue = str.str();
    type = PGE_CVAR_INT;
}

void PGEcfgVariable::Set(const char* value)
{
    sValue = value;
    type = PGE_CVAR_STRING;
}

/**
    Equals to.
    @return True if the cvar can be interpreted as the same int as the given int, false otherwise.
            If the cvar is bool-typed, the result will depend on whether value is true or false and the int is 0 or non-0.
*/
bool PGEcfgVariable::operator==(const int& other) const 
{
    if ( getType() == PGE_CVAR_BOOL )
    {
        if ( other == 0 )
            return (getAsBool() == false);
        return (getAsBool() == true);
    }
    return getAsInt() == other;
}

/**
    Equals to.
    @return True if the cvar can be interpreted as the same float as the given float, false otherwise.
            If the cvar is bool-typed, the result will depend on whether value is true or false and the float is 0 or non-0.
*/
bool PGEcfgVariable::operator==(const float& other) const 
{
    if ( getType() == PGE_CVAR_BOOL )
    {
        if ( other == 0.0f )
            return (getAsBool() == false);
        return (getAsBool() == true);
    }
    return getAsFloat() == other;
}

/**
    Equals to.
    @return True if the cvar can be interpreted as the same bool as the given bool, false otherwise.
*/
bool PGEcfgVariable::operator==(const bool& other) const 
{
    return getAsBool() == other;
}

/**
    Equals to.
    @return True if the cvar can be interpreted as the same string as the given string, false otherwise.
            If the cvar is bool-typed, the result will depend on whether the given string can be interpreted as the same bool.
*/
bool PGEcfgVariable::operator==(const std::string& other) const 
{
    if ( getType() == PGE_CVAR_BOOL )
    {
        const PGEcfgVariable tmp(other.c_str());
        return ( tmp.getAsBool() == getAsBool() );
    }
    return getAsString() == other;
}

/**
    Equals to.
    @return True if not only the value but also the type of the cvars are equal, false otherwise.
*/
bool PGEcfgVariable::operator==(const PGEcfgVariable& other) const 
{
    return (other.sValue == sValue) && (other.type == type);
}

/**
    Not equals to.
*/
bool PGEcfgVariable::operator!=(const int& other) const 
{
    return !( *this == other );
} 

/**
    Not equals to.
*/
bool PGEcfgVariable::operator!=(const float& other) const 
{
    return !( *this == other );
} 

/**
    Not equals to.
*/
bool PGEcfgVariable::operator!=(const bool& other) const 
{
    return !( *this == other );
} 

/**
    Not equals to.
*/
bool PGEcfgVariable::operator!=(const std::string& other) const 
{
    return !( *this == other );
} 

/**
    Not equals to.
*/
bool PGEcfgVariable::operator!=(const PGEcfgVariable& other) const 
{
    return !( *this == other );
}           


/**
    Assignment.
*/
PGEcfgVariable& PGEcfgVariable::operator=(const PGEcfgVariable& other) 
{
    sValue = other.sValue;
    type = other.type;
    return *this;
}        

/**
    Addition.
    Addition of bool-typed cvars is not supported.
    Addition is supported for int-, float- and string-typed cvars.

    @return The returned cvar highly depends on the type of the operands and is determined in the following order:
            If the type of at least one of the cvars is bool, the result will be a copy of the first cvar.
            If the type of at least one of the cvars is string, the result will be a concatenated string form
            of the 2 cvars.
            If the type of at least one of the cvars is float, the result will be float-typed.
            Otherwise, the result will be int-typed.

*/
PGEcfgVariable PGEcfgVariable::operator+(const PGEcfgVariable& other) const 
{
    if ( (getType() == PGE_CVAR_BOOL) || (other.getType() == PGE_CVAR_BOOL) )
        return *this;

    if ( (getType() == PGE_CVAR_STRING) || (other.getType() == PGE_CVAR_STRING) )
        return PGEcfgVariable( getAsString().append(other.getAsString()).c_str() );
    
    if ( (getType() == PGE_CVAR_FLOAT) || (other.getType() == PGE_CVAR_FLOAT) )
        return PGEcfgVariable( getAsFloat() + other.getAsFloat() );

    return PGEcfgVariable( getAsInt() + other.getAsInt() );
}  

/**
    Addition assignment.
*/
PGEcfgVariable& PGEcfgVariable::operator+=(const PGEcfgVariable& other) 
{
    *this = *this + other;
    return *this;
}        


/**
    Subtraction.
    Subtraction of bool-typed and string-typed cvars is not supported.
    Subtraction is supported for int- and float-typed cvars.

    @return The returned cvar highly depends on the type of the operands and is determined in the following order:
            If the type of at least one of the cvars is bool or string, the result will be a copy of the first cvar.
            If the type of at least one of the cvars is float, the result will be float-typed.
            Otherwise, the result will be int-typed.
*/
PGEcfgVariable PGEcfgVariable::operator-(const PGEcfgVariable& other) const 
{
    if ( (getType() == PGE_CVAR_BOOL) || (other.getType() == PGE_CVAR_BOOL) )
        return *this;

    if ( (getType() == PGE_CVAR_STRING) || (other.getType() == PGE_CVAR_STRING) )
        return *this;
    
    if ( (getType() == PGE_CVAR_FLOAT) || (other.getType() == PGE_CVAR_FLOAT) )
        return PGEcfgVariable( getAsFloat() - other.getAsFloat() );

    return PGEcfgVariable( getAsInt() - other.getAsInt() );
}  

/**
    Subtraction assignment.
*/
PGEcfgVariable& PGEcfgVariable::operator-=(const PGEcfgVariable& other) 
{
    *this = *this - other;
    return *this;
}        


/**
    Multiplication by scalar.
    Multiplication of bool-typed and string-typed cvars is not supported.
    Multiplication is supported for int- and float-typed cvars.

    @return The returned cvar highly depends on the type of the operands and is determined in the following order:
            If the type of at least one of the cvars is bool or string, the result will be a copy of the first cvar.
            If the type of at least one of the cvars is float, the result will be float-typed.
            Otherwise, the result will be int-typed.
*/
PGEcfgVariable PGEcfgVariable::operator*(const PGEcfgVariable& other) const 
{
    if ( (getType() == PGE_CVAR_BOOL) || (other.getType() == PGE_CVAR_BOOL) )
        return *this;

    if ( (getType() == PGE_CVAR_STRING) || (other.getType() == PGE_CVAR_STRING) )
        return *this;
    
    if ( (getType() == PGE_CVAR_FLOAT) || (other.getType() == PGE_CVAR_FLOAT) )
        return PGEcfgVariable( getAsFloat() * other.getAsFloat() );

    return PGEcfgVariable( getAsInt() * other.getAsInt() );
}  

/**
    Multiplication assignment.
*/
PGEcfgVariable& PGEcfgVariable::operator*=(const PGEcfgVariable& other) 
{
    *this = *this * other;
    return *this;
}        


/**
    Division by scalar.
    Division of bool-typed and string-typed cvars is not supported.
    Division is supported for int- and float-typed cvars.

    @return The returned cvar highly depends on the type of the operands and is determined in the following order:
            If the type of at least one of the cvars is bool or string, the result will be a copy of the first cvar.
            If the divider is 0, the result will be a copy of the first cvar.
            If the type of at least one of the cvars is float, the result will be float-typed.
            Otherwise, the result will be int-typed.
*/
PGEcfgVariable PGEcfgVariable::operator/(const PGEcfgVariable& other) const 
{
    if ( (getType() == PGE_CVAR_BOOL) || (other.getType() == PGE_CVAR_BOOL) )
        return *this;

    if ( (getType() == PGE_CVAR_STRING) || (other.getType() == PGE_CVAR_STRING) )
        return *this;

    if ( other.getAsFloat() == 0.0f )
        return *this;
    
    if ( (getType() == PGE_CVAR_FLOAT) || (other.getType() == PGE_CVAR_FLOAT) )
         return PGEcfgVariable( getAsFloat() / other.getAsFloat() );

    return PGEcfgVariable( getAsInt() / other.getAsInt() );
}  

/**
    Division assignment.
*/
PGEcfgVariable& PGEcfgVariable::operator/=(const PGEcfgVariable& other) 
{
    *this = *this / other;
    return *this;
}        


/**
    Modulo division by scalar.
    Modulo division is supported for int-typed cvars only.

    @return The returned cvar highly depends on the type of the operands and is determined in the following order:
            If the type of at least one of the operands is not int, the result will be a copy of the first cvar.
            If the divider is 0, the result will be a copy of the first cvar.
            Otherwise, the result will be int-typed.
*/
PGEcfgVariable PGEcfgVariable::operator%(const PGEcfgVariable& other) const 
{
    if ( (getType() == PGE_CVAR_INT) && (other.getType() == PGE_CVAR_INT) )
        if ( other.getAsInt() != 0 )
            return PGEcfgVariable( getAsInt() % other.getAsInt() );

    return *this;
}  

/**
    Modulo division assignment.
*/
PGEcfgVariable& PGEcfgVariable::operator%=(const PGEcfgVariable& other) 
{
    *this = *this % other;
    return *this;
}


/**
    Prefix increment.
    This only works if the type of the cvar is int.
*/
PGEcfgVariable& PGEcfgVariable::operator++() 
{
    if ( getType() == PGE_CVAR_INT )
        Set( getAsInt()+1 );
    return *this;
}

/**
    Postfix increment.
    This only works if the type of the cvar is int.
*/
PGEcfgVariable PGEcfgVariable::operator++(int) 
{
    if ( getType() == PGE_CVAR_INT )
    {
        PGEcfgVariable result = *this;
        Set( getAsInt()+1 );
        return result;
    }
    return *this;
}

/**
    Prefix decrement.
    This only works if the type of the cvar is int.
*/
PGEcfgVariable& PGEcfgVariable::operator--() 
{
    if ( getType() == PGE_CVAR_INT )
        Set( getAsInt()-1 );
    return *this;
}

/**
    Postfix decrement.
    This only works if the type of the cvar is int.
*/
PGEcfgVariable PGEcfgVariable::operator--(int) 
{
    if ( getType() == PGE_CVAR_INT )
    {
        PGEcfgVariable result = *this;
        Set( getAsInt()-1 );
        return result;
    }
    return *this;
} 


/**
    Equals to.
    @return See details at the class-member operator==().
*/
bool operator==(const int& other, const PGEcfgVariable& value)
{
    if ( value.getType() == PGE_CVAR_FLOAT )
        return other == value.getAsFloat();
    if ( value.getType() == PGE_CVAR_BOOL )
    {
        if ( other == 0 )
            return (value.getAsBool() == false);
        return (value.getAsBool() == true);
    }
    return other == value.getAsInt();
}

/**
    Equals to.
    @return See details at the class-member operator==().
*/
bool operator==(const float& other, const PGEcfgVariable& value)
{
    if ( value.getType() == PGE_CVAR_BOOL )
    {
        if ( other == 0.0f )
            return (value.getAsBool() == false);
        return (value.getAsBool() == true);
    }
    return other == value.getAsFloat();
}

/**
    Equals to.
    @return See details at the class-member operator==().
*/
bool operator==(const bool& other, const PGEcfgVariable& value)
{
    return other == value.getAsBool();
}

/**
    Equals to.
    @return See details at the class-member operator==().
*/
bool operator==(const string& other, const PGEcfgVariable& value)
{
    if ( value.getType() == PGE_CVAR_BOOL )
    {
        const PGEcfgVariable tmp(other.c_str());
        return ( tmp.getAsBool() == value.getAsBool() );
    }
    return other == value.getAsString();
}


/**
    Not equals to.
    @return See details at the class-member operator!=().
*/
bool operator!=(const int& other, const PGEcfgVariable& value)
{
    return !(other == value);
}

/**
    Not equals to.
    @return See details at the class-member operator!=().
*/
bool operator!=(const float& other, const PGEcfgVariable& value)
{
    return !(other == value);
}

/**
    Not equals to.
    @return See details at the class-member operator!=().
*/
bool operator!=(const bool& other, const PGEcfgVariable& value)
{
    return !(other == value);
}

/**
    Not equals to.
    @return See details at the class-member operator!=().
*/
bool operator!=(const string& other, const PGEcfgVariable& value)
{
    return !(other == value);
}


/**
    Negative operator.
    Works only if the type of the cvar is int or float.
*/
PGEcfgVariable operator-(const PGEcfgVariable& var)
{
    if ( (var.getType() == PGE_CVAR_INT) || (var.getType() == PGE_CVAR_FLOAT) )
    {
        return PGEcfgVariable( 0 - var );
    }
    return PGEcfgVariable( var );
}


/**
    Addition to scalar.
    @return See class-member operator+() for details.
*/
PGEcfgVariable operator+(const int& other, const PGEcfgVariable& value)
{
    if ( value.getType() == PGE_CVAR_STRING )
    {
        PGEcfgVariable result(other);
        result += value;
        return result;
    }
    return value + other;
}

/**
    Addition to scalar.
    @return See class-member operator+() for details.
*/
PGEcfgVariable operator+(const float& other, const PGEcfgVariable& value)
{
    if ( value.getType() == PGE_CVAR_STRING )
    {
        PGEcfgVariable result(other);
        result += value;
        return result;
    }
    return value + other;
}

/**
    Addition to string.
    @return See class-member operator+() for details.
*/
PGEcfgVariable operator+(const string& other, const PGEcfgVariable& value)
{
    string tmp( other );
    tmp.append( value.getAsString() );
    return PGEcfgVariable( tmp.c_str() );
}


/**
    Subtraction from scalar.
    @return See class-member operator-() for details.
*/
PGEcfgVariable operator-(const int& other, const PGEcfgVariable& value)
{
    if ( (value.getType() == PGE_CVAR_BOOL) || (value.getType() == PGE_CVAR_STRING) )
        return other;
    
    if ( value.getType() == PGE_CVAR_FLOAT )
        return PGEcfgVariable( other - value.getAsFloat() );

    return PGEcfgVariable( other - value.getAsInt() );
}

/**
    Subtraction from scalar.
    @return See class-member operator-() for details.
*/
PGEcfgVariable operator-(const float& other, const PGEcfgVariable& value)
{
    if ( (value.getType() == PGE_CVAR_BOOL) || (value.getType() == PGE_CVAR_STRING) )
        return other;

    return PGEcfgVariable( other - value.getAsFloat() );
}


/**
    Multiplicating scalar.
    @return See class-member operator*() for details.
*/
PGEcfgVariable operator*(const int& other, const PGEcfgVariable& value)
{
    return value * other;
}

/**
    Multiplicating scalar.
    @return See class-member operator*() for details.
*/
PGEcfgVariable operator*(const float& other, const PGEcfgVariable& value)
{
    return value * other;
}


/**
    Dividing scalar.
    @return See class-member operator/() for details.
*/
PGEcfgVariable operator/(const int& other, const PGEcfgVariable& value)
{
    if ( (value.getType() == PGE_CVAR_BOOL) || (value.getType() == PGE_CVAR_STRING) )
        return other;

    const float fDivider = value.getAsFloat();
    if ( fDivider == 0.0f )
        return other;
    
    if ( value.getType() == PGE_CVAR_FLOAT )
        return PGEcfgVariable( other / fDivider );

    return PGEcfgVariable( other / value.getAsInt() );
}

/**
    Dividing scalar.
    @return See class-member operator/() for details.
*/
PGEcfgVariable operator/(const float& other, const PGEcfgVariable& value)
{
    if ( (value.getType() == PGE_CVAR_BOOL) || (value.getType() == PGE_CVAR_STRING) )
        return other;

    const float fDivider = value.getAsFloat();
    if ( fDivider == 0.0f )
        return other;
    
    return PGEcfgVariable( other / fDivider );
}


/**
    Modulo dividing scalar.
    @return See class-member operator%() for details.
*/
PGEcfgVariable operator%(const int& other, const PGEcfgVariable& value)
{
    if ( (value.getType() == PGE_CVAR_INT) && (value.getAsInt() != 0) )
        return PGEcfgVariable( other % value.getAsInt() );

    return other;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


