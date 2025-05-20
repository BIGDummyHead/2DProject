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

    SDL_Rect render(const draw &drawTool, Vector2 where) override;

    void moveRowDown();
    void moveRowUp();

    void moveColRight();
    void moveColLeft();

    int getCurrentRow();
    int getCurrentCol();

    void setRow(int rowIndex);
    void setCol(int colIndex);

};



#endif //SHEET_H
