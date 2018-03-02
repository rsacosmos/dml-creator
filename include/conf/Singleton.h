#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <QMutex>
/*
 * For a definite implementation of the singleton check out
 * the book "Modern C++ Design" by A.Alexandrescu
 */

#ifndef NULL
#  define NULL (0)
#endif // NULL

template <class T>
class Singleton
{
public:
    static T* instance(bool erase = false)
    {
        static QMutex s_mutex;
        static T* s_instance = NULL;
        if (!s_instance)
        {
            s_mutex.lock();
            if (!s_instance)
            {
                s_instance = new T();
            }
            s_mutex.unlock();
        }
        if (erase)
        {
            if(s_instance)
            {
                delete s_instance;
                s_instance = NULL;
            }
        }
        return s_instance;
    }

protected:
    Singleton() {}                          // ctor hidden
    ~Singleton() {}                         // dtor hidden

private:
    Singleton(Singleton const&);            // copy ctor hidden
    Singleton& operator=(Singleton const&); // assign op hidden
};


#endif // _SINGLETON_H_
