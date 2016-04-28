#ifndef BLOCKS
#define BLOCKS

#include <QString>

struct Block {
    QString name;
    unsigned char top, bottom, front, back, left, right;
    bool transparent;
    bool passable;
};

class Blocks
{
public:
    Blocks();
    virtual ~Blocks();

    Block* getBlock(unsigned char type);

    int getTexelDimensions();
protected:
    QString m_texture;
    int m_texelDimensions;
    Block* m_blocks;
};

#endif // BLOCKS

