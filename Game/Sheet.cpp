//
// Created by shawn on 5/16/2025.
//

#include "Sheet.h"

void Sheet::render(draw drawTool, Vector2 where) {
    drawTool.blitSheet(loadedTexture, initRows, initCols, currentRow, currentCol, where, scale);
}


void Sheet::moveColRight() {
    if(currentCol + 1 > initCols - 1)
        return;

    currentCol++;
}

void Sheet::moveColLeft() {
    if(currentCol - 1 < 0)
        return;

    currentCol--;
}

void Sheet::moveRowUp() {
    if(currentRow - 1 < 0)
        return;

    currentRow--;
}

void Sheet::moveRowDown() {
    if(currentRow + 1  > initRows - 1) {
        return;
    }

    currentRow++;
}


int Sheet::getCurrentCol() {
    return currentCol;
}

int Sheet::getCurrentRow() {
    return currentRow;
}

void Sheet::setCol(int colIndex) {
    if(colIndex < 0)
        colIndex = 0;
    else if(colIndex > initCols - 1)
        colIndex = initCols - 1;

    currentCol = colIndex;
}

void Sheet::setRow(int rowIndex) {
    if(rowIndex < 0)
        rowIndex = 0;
    else if(rowIndex > initRows - 1)
        rowIndex = initRows - 1;

    currentRow = rowIndex;
}
