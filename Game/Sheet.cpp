//
// Created by shawn on 5/16/2025.
//

#include "Sheet.h"

void Sheet::render(Vector2 where) {
    renderedTexture = Draw::getInstance()->blitSheet(loadedTexture, initRows, initCols, currentRow, currentCol, where, scale);
}


void Sheet::moveColRight(const bool loop) {
    if(currentCol + 1 > initCols - 1) {

        if(loop)
            currentCol = 0;

        return;
    }

    currentCol++;
}

void Sheet::moveColLeft(const bool loop) {
    if(currentCol - 1 < 0) {

        if(loop)
            currentCol = initCols - 1;

        return;
    }

    currentCol--;
}

void Sheet::moveRowUp(const bool loop) {
    if(currentRow - 1 < 0) {

        if(loop) {
            currentRow = initRows - 1;
        }

        return;
    }

    currentRow--;
}

void Sheet::moveRowDown(const bool loop) {
    if(currentRow + 1  > initRows - 1) {

        if(loop) {
            currentRow = 0;
        }

        return;
    }

    currentRow++;
}


int Sheet::getCurrentCol() const {
    return currentCol;
}

int Sheet::getCurrentRow() const {
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

int Sheet::totalCols() const {
    return initCols;
}

int Sheet::totalRows() const {
    return initRows;
}


