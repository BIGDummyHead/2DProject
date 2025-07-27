//
// Created by shawn on 5/16/2025.
//

#ifndef SHEET_H
#define SHEET_H
#include "Texture.h"


class Sheet : public Texture {

private:
    int initRows;
    int initCols;

    int currentRow;
    int currentCol;

public:
    Sheet(SDL_Texture* txture, int rows, int cols) : Texture(txture) {
        initRows = rows;
        initCols = cols;
        currentCol = 0;
        currentRow = 0;
        scale = Vector2(1,1);
    }

    Vector2 scale;

    void render(Vector2 where) override;

    void moveRowDown(bool loop = false);
    void moveRowUp(bool loop = false);

    void moveColRight(bool loop = false);
    void moveColLeft(bool loop = false);

    [[nodiscard]] int getCurrentRow() const;
    [[nodiscard]] int getCurrentCol() const;

    void setRow(int rowIndex);
    void setCol(int colIndex);

    [[nodiscard]] int totalRows() const;
    [[nodiscard]] int totalCols() const;

};



#endif //SHEET_H
