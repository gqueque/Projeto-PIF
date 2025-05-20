/**
 * screen.c
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
*/

#include "screen.h"

void screenDrawBorders() 
{
    char hbc = BOX_HLINE;
    char vbc = BOX_VLINE;
    
    screenClear();
    screenBoxEnable();
    
    screenGotoxy(MINX, MINY);
    printf("%c", BOX_UPLEFT);

    for (int i=MINX+1; i<MAXX; i++)
    {
        screenGotoxy(i, MINY);
        printf("%c", hbc);
    }
    screenGotoxy(MAXX, MINY);
    printf("%c", BOX_UPRIGHT);

    for (int i=MINY+1; i<MAXY; i++)
    {
        screenGotoxy(MINX, i);
        printf("%c", vbc);
        screenGotoxy(MAXX, i);
        printf("%c", vbc);
    }

    screenGotoxy(MINX, MAXY);
    printf("%c", BOX_DWNLEFT);
    for (int i=MINX+1; i<MAXX; i++)
    {
        screenGotoxy(i, MAXY);
        printf("%c", hbc);
    }
    screenGotoxy(MAXX, MAXY);
    printf("%c", BOX_DWNRIGHT);

    screenBoxDisable();
}

void screenDrawBox(int x1, int y1, int x2, int y2) {
    screenGotoxy(x1, y1); printf("┌");
    screenGotoxy(x2, y1); printf("┐");
    screenGotoxy(x1, y2); printf("└");
    screenGotoxy(x2, y2); printf("┘");

    for (int x = x1 + 1; x < x2; x++) {
        screenGotoxy(x, y1); printf("─");
        screenGotoxy(x, y2); printf("─");
    }

    for (int y = y1 + 1; y < y2; y++) {
        screenGotoxy(x1, y); printf("│");
        screenGotoxy(x2, y); printf("│");
    }
}

void screenInit(int drawBorders)
{
    screenClear();
    if (drawBorders) screenDrawBorders();
    screenHomeCursor();
    screenHideCursor();
}

void screenDestroy()
{
    printf("%s[0;39;49m", ESC); 
    screenSetNormal();
    screenClear();
    screenHomeCursor();
    screenShowCursor();
}

void screenGotoxy(int x, int y)
{
    x = ( x<0 ? 0 : x>=MAXX ? MAXX-1 : x);
    y = ( y<0 ? 0 : y>MAXY ? MAXY : y);
    
    printf("%s[f%s[%dB%s[%dC", ESC, ESC, y, ESC, x);
}

void screenSetColor( screenColor fg, screenColor bg)
{
    char atr[] = "[0;";

    if ( fg > LIGHTGRAY )
    {
        atr[1] = '1';
		fg -= 8;
    }

    printf("%s%s%d;%dm", ESC, atr, fg+30, bg+40);
}
