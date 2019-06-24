#pragma once

/*
    ###################################################################################
    PGEcfgVariable.h
    This file is part of PGE.
    Internal header.
    PR00F's Game Engine cfg variable (cvar) class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PGEallHeaders.h"


/**
    Type of a PGEcfgVariable (cvar).
    Type depends on how the stored value could be represented the best way.
    It depends on the last non-const method called.
    For example, if you construct the object with an int, the type will be PGE_CVAR_INT.
    If you multiply this object by a float, the type will change to PGE_CVAR_FLOAT.
    Not all methods are valid for all types, for example you cannot multiply a string-typed cvar
    with an int-typed cvar because it has no sense.
*/
enum TPGE_CFG_VARIABLE_TYPE
{
    PGE_CVAR_INT,     /**< Type of cvar object is integer. */
    PGE_CVAR_FLOAT,   /**< Type of cvar object is floating point number. */
    PGE_CVAR_BOOL,    /**< Type of cvar object is boolean. */
    PGE_CVAR_STRING   /**< Type of cvar object is string. */
};

/**

    PR00F's Game Engine cfg variable (cvar) class.
    This class can hold a value of an arbitrary type.

    This class doesn't use any API directly.

*/
class PGEcfgVariable
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGEcfgVariable is included")   
#endif

public:
    PGEcfgVariable();                      /**< Constructs a string-typed cvar. */

    // ctors below are intentionally not explicit because we allow implicit conversion
    PGEcfgVariable(const int& value);      /**< Constructs an integer-typed cvar. */
    PGEcfgVariable(const float& value);    /**< Constructs a floating point-typed cvar. */
    PGEcfgVariable(const bool& value);     /**< Constructs a boolean-typed cvar. */
    PGEcfgVariable(const char* value);     /**< Constructs a string-typed cvar. */
    virtual ~PGEcfgVariable();

    int         getAsInt() const;          /**< Tries to return the value of the cvar as an integer. */
    float       getAsFloat() const;        /**< Tries to return the value of the cvar as a floating point number. */
    bool        getAsBool() const;         /**< Returns the value of the cvar as a boolean. */
    std::string getAsString() const;       /**< Returns the value of the cvar as a string. */

    TPGE_CFG_VARIABLE_TYPE getType() const;  /**< Returns the type of the cvar. */

    void Set(int value);
    void Set(float value);
    void Set(bool value);
    void Set(const char* value);

    bool   operator==(const int& other) const;                      /**< Equals to. */
    bool   operator==(const float& other) const;                    /**< Equals to. */
    bool   operator==(const bool& other) const;                     /**< Equals to. */
    bool   operator==(const std::string& other) const;              /**< Equals to. */
    bool   operator==(const PGEcfgVariable& other) const;           /**< Equals to. */
    bool   operator!=(const int& other) const;                      /**< Not equals to. */
    bool   operator!=(const float& other) const;                    /**< Not equals to. */
    bool   operator!=(const bool& other) const;                     /**< Not equals to. */
    bool   operator!=(const std::string& other) const;              /**< Not equals to. */
    bool   operator!=(const PGEcfgVariable& other) const;           /**< Not equals to. */
    PGEcfgVariable& operator= (const PGEcfgVariable& other);        /**< Assignment. */
    PGEcfgVariable  operator+ (const PGEcfgVariable& other) const;  /**< Addition. */
    PGEcfgVariable& operator+=(const PGEcfgVariable& other);        /**< Addition assignment. */
    PGEcfgVariable  operator- (const PGEcfgVariable& other) const;  /**< Subtraction. */
    PGEcfgVariable& operator-=(const PGEcfgVariable& other);        /**< Subtraction assignment. */
    PGEcfgVariable  operator* (const PGEcfgVariable& other) const;  /**< Multiplication by scalar. */
    PGEcfgVariable& operator*=(const PGEcfgVariable& other);        /**< Multiplication assignment. */
    PGEcfgVariable  operator/ (const PGEcfgVariable& other) const;  /**< Division by scalar. */
    PGEcfgVariable& operator/=(const PGEcfgVariable& other);        /**< Division assignment. */
    PGEcfgVariable  operator% (const PGEcfgVariable& other) const;  /**< Modulo division by scalar. */
    PGEcfgVariable& operator%=(const PGEcfgVariable& other);        /**< Modulo division assignment. */
    PGEcfgVariable& operator++();     /**< Prefix increment. */
    PGEcfgVariable  operator++(int);  /**< Postfix increment. */
    PGEcfgVariable& operator--();     /**< Prefix decrement. */
    PGEcfgVariable  operator--(int);  /**< Postfix decrement. */

protected:

private:
    std::string sValue;
    TPGE_CFG_VARIABLE_TYPE type;

    // ---------------------------------------------------------------------------


};

bool operator==(const int& other,         const PGEcfgVariable& value);  /**< Equals to. */
bool operator==(const float& other,       const PGEcfgVariable& value);  /**< Equals to. */
bool operator==(const bool& other,        const PGEcfgVariable& value);  /**< Equals to. */
bool operator==(const std::string& other, const PGEcfgVariable& value);  /**< Equals to. */
bool operator!=(const int& other,         const PGEcfgVariable& value);  /**< Not equals to. */
bool operator!=(const float& other,       const PGEcfgVariable& value);  /**< Not equals to. */
bool operator!=(const bool& other,        const PGEcfgVariable& value);  /**< Not equals to. */
bool operator!=(const std::string& other, const PGEcfgVariable& value);  /**< Not equals to. */
PGEcfgVariable operator-(const PGEcfgVariable& value);                            /**< Negative operator. */
PGEcfgVariable operator+(const int& other,         const PGEcfgVariable& value);  /**< Addition to scalar. */
PGEcfgVariable operator+(const float& other,       const PGEcfgVariable& value);  /**< Addition to scalar. */
PGEcfgVariable operator+(const std::string& other, const PGEcfgVariable& value);  /**< Addition to string. */
PGEcfgVariable operator-(const int& other,         const PGEcfgVariable& value);  /**< Subtraction from scalar. */
PGEcfgVariable operator-(const float& other,       const PGEcfgVariable& value);  /**< Subtraction from scalar. */
PGEcfgVariable operator*(const int& other,         const PGEcfgVariable& value);  /**< Multiplicating scalar. */
PGEcfgVariable operator*(const float& other,       const PGEcfgVariable& value);  /**< Multiplicating scalar. */
PGEcfgVariable operator/(const int& other,         const PGEcfgVariable& value);  /**< Dividing scalar. */
PGEcfgVariable operator/(const float& other,       const PGEcfgVariable& value);  /**< Dividing scalar. */
PGEcfgVariable operator%(const int& other,         const PGEcfgVariable& value);  /**< Modulo dividing scalar. */

