#ifndef CELL_HPP
#define CELL_HPP

class Cell {
public:
    virtual char getSymbol() const = 0; 
    virtual bool isWalkable() const = 0;
    virtual bool isDirty() const { return false; }
    virtual void clean() {}
    virtual void reset() {} 
    virtual ~Cell() {}
};

class Wall : public Cell {
public:
    char getSymbol() const override { return '#'; }
    bool isWalkable() const override { return false; }
};

class Obstacle : public Cell {
public:
    char getSymbol() const override { return 'X'; }
    bool isWalkable() const override { return false; }
};

class Dirt : public Cell {
private:
    bool cleaned = false;
public:
    char getSymbol() const override { return cleaned ? '.' : 'O'; }
    bool isWalkable() const override { return true; }
    bool isDirty() const override { return !cleaned; }
    void clean() override { cleaned = true; }
    void reset() override { cleaned = false; }
};

class Empty : public Cell {
private:
    bool visited = false;
public:
    char getSymbol() const override { return visited ? '.' : ' '; }
    bool isWalkable() const override { return true; }
    void clean() override { visited = true; }
    void reset() override { visited = false; }
};
#endif