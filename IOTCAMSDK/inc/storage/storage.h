#ifndef __storage_h__
#define __storage_h__


class storage
{
public:
    storage();
    virtual ~storage();

    virtual int save(unsigned char *data , int len);
    virtual int setPath(char *path);
    virtual char* getPath();

protected:

    char m_path[128];

};

#endif


