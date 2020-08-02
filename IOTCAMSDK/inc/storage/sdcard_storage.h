#include "storage.h"


class SDStorage : public storage {

public:
    SDStorage();
    ~SDStorage();

    int checkMount();

protected:

    virtual int setPath(char *path);
    virtual char *getPath();
    virtual int save(unsigned char *data , int len);

private:

    int cunchushijian; //
    int curOpenFd;




};