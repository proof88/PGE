#pragma once

/*
    ###################################################################################
    PgeOldNewValue.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine old-new variable class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PGEallHeaders.h"

#include <ostream>

/**
    PR00F's Game Engine old-new variable class.
    This class can remember its old (previous) value.
    Its current value is always its new value.
    Useful if you want to implement a mechanism that depends on comparison of old and new value, such as:
     - configuration: old value is the applied, new value is to be applied, saving and applying is needed only if old is different than new;
     - sending updates over network: current values are saved at the beginning of a frame, and by end of frame sending any update over network
       is needed only if the old values are different to the current values.
*/
template <typename T>
class PgeOldNewValue
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeOldNewValue is included")   
#endif

public:
    /**
    * Constructs an object with both old and new values initialized to their default values based on their type.
    * By default dirty flag is cleared.
    */
    PgeOldNewValue();

    /**
    * Constructs an object with both old and new values as the given value.
    * By default dirty flag is cleared.
    */
    explicit PgeOldNewValue(const T& value);

    /**
    * @return Old (previous) value.
    */
    const T& getOld() const;

    /**
    * @return New (current) value.
    */
    const T& getNew() const;

    /**
    * Sets the given value as new value.
    * @return Value of isDirty() after setting the new value.
    */
    bool set(const T& value);

    /**
    * Dirty flag.
    * @return True (dirty flag set) if old value is different to new value, false (dirty flag cleared) otherwise.
    */
    bool isDirty() const;

    /**
    * Resets new value to be same as old value and clears dirty flag.
    */
    void revert();

    /**
    * Saves new value to old value and clears dirty flag.
    */
    void commit();

    bool operator==(const T& other) const;                                /**< Equals to value-type. */
    bool operator==(const PgeOldNewValue<T>& other) const;                /**< Equals to. */
    bool operator!=(const T& other) const;                                /**< Not equals to value-type. */
    bool operator!=(const PgeOldNewValue<T>& other) const;                /**< Not equals to. */
    PgeOldNewValue<T>& PgeOldNewValue<T>::operator=(const T& other);      /**< Assignment to value-type. */
    PgeOldNewValue<T>  operator+ (const T& other) const;                  /**< Addition. */
    PgeOldNewValue<T>  operator+ (const PgeOldNewValue<T>& other) const;  /**< Addition. */
    PgeOldNewValue<T>& operator+=(const T& other);                        /**< Addition assignment. */
    PgeOldNewValue<T>& operator+=(const PgeOldNewValue<T>& other);        /**< Addition assignment. */
    PgeOldNewValue<T>  operator- (const T& other) const;                  /**< Subtraction. */
    PgeOldNewValue<T>  operator- (const PgeOldNewValue<T>& other) const;  /**< Subtraction. */
    PgeOldNewValue<T>& operator-=(const T& other);                        /**< Subtraction assignment. */
    PgeOldNewValue<T>& operator-=(const PgeOldNewValue<T>& other);        /**< Subtraction assignment. */
    PgeOldNewValue<T>  operator* (const T& other) const;                  /**< Multiplication by scalar. */
    PgeOldNewValue<T>  operator* (const PgeOldNewValue<T>& other) const;  /**< Multiplication by scalar. */
    PgeOldNewValue<T>& operator*=(const T& other);                        /**< Multiplication assignment. */
    PgeOldNewValue<T>& operator*=(const PgeOldNewValue<T>& other);        /**< Multiplication assignment. */
    PgeOldNewValue<T>  operator/ (const T& other) const;                  /**< Division by scalar. */
    PgeOldNewValue<T>  operator/ (const PgeOldNewValue<T>& other) const;  /**< Division by scalar. */
    PgeOldNewValue<T>& operator/=(const T& other);                        /**< Division assignment. */
    PgeOldNewValue<T>& operator/=(const PgeOldNewValue<T>& other);        /**< Division assignment. */
    PgeOldNewValue<T>  operator% (const T& other) const;                  /**< Modulo division by scalar. */
    PgeOldNewValue<T>  operator% (const PgeOldNewValue<T>& other) const;  /**< Modulo division by scalar. */
    PgeOldNewValue<T>& operator%=(const T& other);                        /**< Modulo division assignment. */
    PgeOldNewValue<T>& operator%=(const PgeOldNewValue<T>& other);        /**< Modulo division assignment. */
    PgeOldNewValue<T>& operator++();      /**< Prefix increment. */
    PgeOldNewValue<T>  operator++(int);   /**< Postfix increment. */
    PgeOldNewValue<T>& operator--();      /**< Prefix decrement. */
    PgeOldNewValue<T>  operator--(int);   /**< Postfix decrement. */

protected:

private:
    T m_newValue;
    T m_oldValue;
    bool m_bDirty;
};

template <typename T>
std::ostream& operator<< (std::ostream& os, const PgeOldNewValue<T>& obj);
template <typename T>
bool operator==(const T& other, const PgeOldNewValue<T>& value);  /**< Equals to value-type. */
template <typename T>
bool operator!=(const T& other, const PgeOldNewValue<T>& value);  /**< Not equals to value-type. */

template <typename T>
PgeOldNewValue<T> operator-(const PgeOldNewValue<T>& value);                  /**< Negative operator. */
template <typename T>
PgeOldNewValue<T> operator+(const T& other, const PgeOldNewValue<T>& value);  /**< Addition. */
template <typename T>
PgeOldNewValue<T> operator-(const T& other, const PgeOldNewValue<T>& value);  /**< Subtraction. */
template <typename T>
PgeOldNewValue<T> operator*(const T& other, const PgeOldNewValue<T>& value);  /**< Multiplication. */
template <typename T>
PgeOldNewValue<T> operator/(const T& other, const PgeOldNewValue<T>& value);  /**< Division. */
template <typename T>
PgeOldNewValue<T> operator%(const T& other, const PgeOldNewValue<T>& value);  /**< Modulo division. */

template <typename T>
PgeOldNewValue<T>::PgeOldNewValue() :
    m_oldValue{},
    m_newValue{},
    m_bDirty(false)
{}

template <typename T>
PgeOldNewValue<T>::PgeOldNewValue(const T& value) :
    m_oldValue(value),
    m_newValue(value),
    m_bDirty(false)
{}

template <typename T>
inline const T& PgeOldNewValue<T>::getOld() const
{
    return m_oldValue;
}

template <typename T>
inline const T& PgeOldNewValue<T>::getNew() const
{
    return m_newValue;
}

template <typename T>
bool PgeOldNewValue<T>::set(const T& value)
{
    m_newValue = value;
    if (m_newValue != m_oldValue)
    {
        m_bDirty = true;
    }
    return m_bDirty;
}

template <typename T>
inline bool PgeOldNewValue<T>::isDirty() const
{
    return m_bDirty;
}

template <typename T>
void PgeOldNewValue<T>::revert()
{
    m_newValue = m_oldValue;
    m_bDirty = false;
}

template <typename T>
void PgeOldNewValue<T>::commit()
{
    m_oldValue = m_newValue;
    m_bDirty = false;
}

/**
    Equals to value-type.
    @return True if our current (new) value equals to the given other value, false otherwise.
*/
template <typename T>
inline bool PgeOldNewValue<T>::operator==(const T& other) const
{
    return getNew() == other;
}

/**
    Equals to.
    @return True if both our old and current (new) values equal to the old and current (new) values of the other given object, false otherwise.
*/
template <typename T>
inline bool PgeOldNewValue<T>::operator==(const PgeOldNewValue<T>& other) const
{
    return (getNew() == other.getNew()) && (getOld() == other.getOld());
}

/**
    Not equals to value-type.
    @return True if our current (new) value does not equal to the given other value, false otherwise.
*/
template <typename T>
inline bool PgeOldNewValue<T>::operator!=(const T& other) const
{
    return !( *this == other );
}   

/**
    Not equals to.
    @return True if any of our old and current (new) values does not equal to the old and current (new) values of the other given object, false otherwise.
*/
template <typename T>
inline bool PgeOldNewValue<T>::operator!=(const PgeOldNewValue<T>& other) const
{
    return !(*this == other);
}

/**
    Assignment to value-type.
    @return This object with its current (new) value set to the given other value.
*/
template <typename T>
inline PgeOldNewValue<T>& PgeOldNewValue<T>::operator=(const T& other)
{
    set(other);
    return *this;
}

/**
    Addition.
    @return A PgeOldNewValue where both old and new value are set to the result of the operation done on the new value of this and the other operands.
*/
template <typename T>
inline PgeOldNewValue<T> PgeOldNewValue<T>::operator+(const T& other) const
{
    return PgeOldNewValue<T>( getNew() + other );
}

/**
    Addition.
    @return A PgeOldNewValue where both old and new value are set to the result of the operation done on the new values of the 2 PgeOldNewValue operands.
*/
template <typename T>
inline PgeOldNewValue<T> PgeOldNewValue<T>::operator+(const PgeOldNewValue<T>& other) const 
{
    return PgeOldNewValue<T>( getNew() + other.getNew() );
}

/**
    Addition assignment.
    @return This object where only the new value is set to the result of the operation done on the new value of this and the other operands.
*/
template <typename T>
inline PgeOldNewValue<T>& PgeOldNewValue<T>::operator+=(const T& other)
{
    set(getNew() + other);
    return *this;
}

/**
    Addition assignment.
    @return This object where only the new value is set to the result of the operation done on the new values of this and the other PgeOldNewValue operands.
*/
template <typename T>
inline PgeOldNewValue<T>& PgeOldNewValue<T>::operator+=(const PgeOldNewValue<T>& other) 
{
    set(getNew() + other.getNew());
    return *this;
}

/**
    Subtraction.
    @return A PgeOldNewValue where both old and new value are set to the result of the operation done on the new value of this and the other operands.
*/
template <typename T>
inline PgeOldNewValue<T> PgeOldNewValue<T>::operator-(const T& other) const
{
    return PgeOldNewValue<T>(getNew() - other);
}

/**
    Subtraction.
    @return A PgeOldNewValue where both old and new value are set to the result of the operation done on the new values of the 2 PgeOldNewValue operands.
*/
template <typename T>
inline PgeOldNewValue<T> PgeOldNewValue<T>::operator-(const PgeOldNewValue<T>& other) const
{
    return PgeOldNewValue<T>(getNew() - other.getNew());
}

/**
    Subtraction assignment.
    @return This object where only the new value is set to the result of the operation done on the new value of this and the other operands.
*/
template <typename T>
inline PgeOldNewValue<T>& PgeOldNewValue<T>::operator-=(const T& other)
{
    set(getNew() - other);
    return *this;
}

/**
    Subtraction assignment.
    @return This object where only the new value is set to the result of the operation done on the new values of this and the other PgeOldNewValue operands.
*/
template <typename T>
inline PgeOldNewValue<T>& PgeOldNewValue<T>::operator-=(const PgeOldNewValue<T>& other)
{
    set(getNew() - other.getNew());
    return *this;
}

/**
    Multiplication.
    @return A PgeOldNewValue where both old and new value are set to the result of the operation done on the new value of this and the other operands.
*/
template <typename T>
inline PgeOldNewValue<T> PgeOldNewValue<T>::operator*(const T& other) const
{
    return PgeOldNewValue<T>(getNew() * other);
}

/**
    Multiplication.
    @return A PgeOldNewValue where both old and new value are set to the result of the operation done on the new values of the 2 PgeOldNewValue operands.
*/
template <typename T>
inline PgeOldNewValue<T> PgeOldNewValue<T>::operator*(const PgeOldNewValue<T>& other) const
{
    return PgeOldNewValue<T>(getNew() * other.getNew());
}

/**
    Multiplication assignment.
    @return This object where only the new value is set to the result of the operation done on the new value of this and the other operands.
*/
template <typename T>
inline PgeOldNewValue<T>& PgeOldNewValue<T>::operator*=(const T& other)
{
    set(getNew() * other);
    return *this;
}

/**
    Multiplication assignment.
    @return This object where only the new value is set to the result of the operation done on the new values of this and the other PgeOldNewValue operands.
*/
template <typename T>
inline PgeOldNewValue<T>& PgeOldNewValue<T>::operator*=(const PgeOldNewValue<T>& other)
{
    set(getNew() * other.getNew());
    return *this;
}

/**
    Division.
    @return A PgeOldNewValue where both old and new value are set to the result of the operation done on the new value of this and the other operands.
*/
template <typename T>
inline PgeOldNewValue<T> PgeOldNewValue<T>::operator/(const T& other) const
{
    return PgeOldNewValue<T>(getNew() / other);
}

/**
    Division.
    @return A PgeOldNewValue where both old and new value are set to the result of the operation done on the new values of the 2 PgeOldNewValue operands.
*/
template <typename T>
inline PgeOldNewValue<T> PgeOldNewValue<T>::operator/(const PgeOldNewValue<T>& other) const
{
    return PgeOldNewValue<T>(getNew() / other.getNew());
}

/**
    Division assignment.
    @return This object where only the new value is set to the result of the operation done on the new value of this and the other operands.
*/
template <typename T>
inline PgeOldNewValue<T>& PgeOldNewValue<T>::operator/=(const T& other)
{
    set(getNew() / other);
    return *this;
}

/**
    Division assignment.
    @return This object where only the new value is set to the result of the operation done on the new values of this and the other PgeOldNewValue operands.
*/
template <typename T>
inline PgeOldNewValue<T>& PgeOldNewValue<T>::operator/=(const PgeOldNewValue<T>& other)
{
    set(getNew() / other.getNew());
    return *this;
}

/**
    Modulo division.
    @return A PgeOldNewValue where both old and new value are set to the result of the operation done on the new value of this and the other operands.
*/
template <typename T>
inline PgeOldNewValue<T> PgeOldNewValue<T>::operator%(const T& other) const
{
    return PgeOldNewValue<T>(getNew() % other);
}

/**
    Modulo division.
    @return A PgeOldNewValue where both old and new value are set to the result of the operation done on the new values of the 2 PgeOldNewValue operands.
*/
template <typename T>
inline PgeOldNewValue<T> PgeOldNewValue<T>::operator%(const PgeOldNewValue<T>& other) const
{
    return PgeOldNewValue<T>(getNew() % other.getNew());
}

/**
    Modulo division assignment.
    @return This object where only the new value is set to the result of the operation done on the new value of this and the other operands.
*/
template <typename T>
inline PgeOldNewValue<T>& PgeOldNewValue<T>::operator%=(const T& other)
{
    set(getNew() % other);
    return *this;
}

/**
    Modulo division assignment.
    @return This object where only the new value is set to the result of the operation done on the new values of this and the other PgeOldNewValue operands.
*/
template <typename T>
inline PgeOldNewValue<T>& PgeOldNewValue<T>::operator%=(const PgeOldNewValue<T>& other)
{
    set(getNew() % other.getNew());
    return *this;
}

/**
    Prefix increment.
    @return This object after the operation, only the new value is incremented.
*/
template <typename T>
inline PgeOldNewValue<T>& PgeOldNewValue<T>::operator++()
{
    T newValue = getNew();
    set(++newValue);
    return *this;
}

/**
    Postfix increment.
    @return Copy of this object before the operation (new value is incremented only in the original object).
*/
template <typename T>
inline PgeOldNewValue<T> PgeOldNewValue<T>::operator++(int)
{
    PgeOldNewValue<T> result = *this;
    T newValue = getNew();
    set(++newValue);
    return result;
}

/**
    Prefix decrement.
    @return This object after the operation, only the new value is decremented.
*/
template <typename T>
inline PgeOldNewValue<T>& PgeOldNewValue<T>::operator--()
{
    T newValue = getNew();
    set(--newValue);
    return *this;
}

/**
    Postfix decrement.
    @return Copy of this object before the operation (new value is decremented only in the original object).
*/
template <typename T>
inline PgeOldNewValue<T> PgeOldNewValue<T>::operator--(int)
{
    PgeOldNewValue<T> result = *this;
    T newValue = getNew();
    set(--newValue);
    return result;
}

template <typename T>
std::ostream& operator<< (std::ostream& os, const PgeOldNewValue<T>& obj)
{
    os << "Old: " << obj.getOld() << ", New: " << obj.getNew();
    return os;
}

/**
    Equals to.
    @return See details at the class-member operator==().
*/
template <typename T>
inline bool operator==(const T& other, const PgeOldNewValue<T>& value)
{
    return value == other;
}

/**
    Not equals to.
    @return See details at the class-member operator!=().
*/
template <typename T>
inline bool operator!=(const T& other, const PgeOldNewValue<T>& value)
{
    return !(other == value);
}

/**
    Negative operator.
*/
template <typename T>
inline PgeOldNewValue<T> operator-(const PgeOldNewValue<T>& value)
{
    return PgeOldNewValue<T>(0 - value.getNew());
}

/**
    Addition.
    @return See class-member operator+() for details.
*/
template <typename T>
inline PgeOldNewValue<T> operator+(const T& other, const PgeOldNewValue<T>& value)
{
    return PgeOldNewValue<T>(other + value.getNew());
}

/**
    Subtraction.
    @return See class-member operator-() for details.
*/
template <typename T>
inline PgeOldNewValue<T> operator-(const T& other, const PgeOldNewValue<T>& value)
{
    return PgeOldNewValue<T>(other - value.getNew());
}

/**
    Multiplication.
    @return See class-member operator*() for details.
*/
template <typename T>
inline PgeOldNewValue<T> operator*(const T& other, const PgeOldNewValue<T>& value)
{
    return PgeOldNewValue<T>(other * value.getNew());
}

/**
    Division.
    @return See class-member operator/() for details.
*/
template <typename T>
inline PgeOldNewValue<T> operator/(const T& other, const PgeOldNewValue<T>& value)
{
    return PgeOldNewValue<T>(other / value.getNew());
}

/**
    Modulo division.
    @return See class-member operator%() for details.
*/
template <typename T>
inline PgeOldNewValue<T> operator%(const T& other, const PgeOldNewValue<T>& value)
{
    return PgeOldNewValue<T>(other % value.getNew());
}
