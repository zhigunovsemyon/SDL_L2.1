#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#ifndef SDL_h_
#include <SDL2/SDL.h>
#endif /* ifndef SDL_h_ */
#include <stdio.h>

//Коды
#define TRUE 1
#define FALSE 0
#define NOERR	0
#define SDLERR	-1
#define MALLOCFAIL	-2
#define NOLETTER -3

//Параметры окна
#define TITLE "Лабораторная работа №3. Задание 1"
#define BGCOLOUR 0xEF, 0x80, 0x00
#define FGCOLOUR 0x00, 0xEF, 0x80
#define COUNT 5

int DrawLetterH(SDL_Renderer* rend, const SDL_Rect* corner, int segment, Uint8 r, Uint8 g, Uint8 b);
int DrawLetterM(SDL_Renderer* rend, const SDL_Rect* corner, int segment, Uint8 r, Uint8 g, Uint8 b);
int DrawLetterE(SDL_Renderer* rend, const SDL_Rect* corner, int segment, Uint8 r, Uint8 g, Uint8 b);
int DrawLetterC(SDL_Renderer* rend, const SDL_Rect* corner, int segment, Uint8 r, Uint8 g, Uint8 b);
int DestroyAllAndExit(SDL_Renderer* rend, SDL_Window* win);
int DrawBackRects(SDL_Renderer* rend, SDL_Rect** rectArr, int count, Uint8 r, Uint8 g, Uint8 b, int size, int spacing);
int DrawBackground(SDL_Renderer* rend, Uint8 r, Uint8 g, Uint8 b);
int CreateAndCheckSpace(SDL_Window** win, SDL_Renderer** rend, int h, int w);
int DrawLetters(SDL_Renderer* rend, const SDL_Rect* backRect, int segment, Uint8 r, Uint8 g, Uint8 b);
int GetSegmentSize(void);

int main(int argc, char* *argv)
{
	//Параметры квадратов
	int segment = GetSegmentSize(),
		sqsize = segment * 5,
		Window_HSize = sqsize * COUNT + segment * (COUNT + 1),
		Window_VSize = sqsize + segment * 2;

	//Создание рабочего пространство
	SDL_Window* win;
	SDL_Renderer* rend;
	SDL_Rect *charBoxes;
	if (CreateAndCheckSpace(&win, &rend, Window_HSize, Window_VSize))
		return SDLERR;
	
	//Заливка фона, завершение работы если не удалось
	if (DrawBackground(rend, BGCOLOUR))
		return DestroyAllAndExit(rend, win);
	SDL_Delay(200);

	//Рисование полей для символов, завершение работы если не удалось
	if (DrawBackRects(rend, &charBoxes, COUNT, FGCOLOUR, sqsize, segment))
		return DestroyAllAndExit(rend, win);
	SDL_RenderPresent(rend);
	SDL_Delay(200);

	//Рисование букв, завершение работы если не удалось
	if (DrawLetters(rend, charBoxes, segment, BGCOLOUR))
		return DestroyAllAndExit(rend, win);
	SDL_RenderPresent(rend);
	SDL_Delay(200);

	SDL_free(charBoxes);
	
	SDL_Delay(2000);
	//Штатное завершение работы
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return NOERR;
}

/*Вызов SDL, создание окна и рисовальщика.
Если что-то не удалось выполнить, вывод ошибки и завершение работы*/
int CreateAndCheckSpace(SDL_Window** win, SDL_Renderer** rend, int h, int w)
{
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		printf("Ошибка SDL: %s", SDL_GetError());
		return SDLERR;
	}
	if (NULL == (*win = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		h, w, SDL_WINDOW_SHOWN)))
	{
		printf("Ошибка SDL: %s", SDL_GetError());
		SDL_Quit();
		return SDLERR;
	}
	if (NULL == (*rend = SDL_CreateRenderer(*win, -1, 0)))
	{
		printf("Ошибка SDL: %s", SDL_GetError());
		SDL_DestroyWindow(*win);
		SDL_Quit();
		return SDLERR;
	}
	/*else*/ return NOERR;
}

/*Функция рисования фона рисовальщика rend, цветом r g b*/
int DrawBackground(SDL_Renderer* rend, Uint8 r, Uint8 g, Uint8 b)
{
	if (SDL_SetRenderDrawColor(rend, r, g, b, 0xFF) ||
		SDL_RenderClear(rend))
	{
		return SDLERR;
	}
	/*else*/
	SDL_RenderPresent(rend);
	return NOERR;
}

/*Функция рисования count квадратов размером size на расстоянии spacing друг от друга
рисовальщиком rend, цветом r g b, в массив квадратов rectArr*/
int DrawBackRects(SDL_Renderer* rend, SDL_Rect** rectArr, int count, Uint8 r, Uint8 g, Uint8 b, int size, int spacing)
{
	*rectArr = (SDL_Rect*)SDL_malloc(sizeof(SDL_Rect) * count);
	if (NULL == *rectArr)
		return MALLOCFAIL;
	for (int i = 0; i < count; ++i)
	{
		(*rectArr + i)->x = spacing + (spacing + size) * i;
		(*rectArr + i)->y = spacing;
		(*rectArr + i)->w = size;
		(*rectArr + i)->h = size;
	}

	if (SDL_SetRenderDrawColor(rend, r, g, b, 0xFF) ||
		SDL_RenderFillRects(rend, *rectArr, count))
	{
		return SDLERR;
	}
	/*else*/
	return NOERR;
}

/*Вывод сообщения об ошибке, удаление рисовальщика rend, окна win, завершение работы SDL*/
int DestroyAllAndExit(SDL_Renderer* rend, SDL_Window* win)
{
	printf("Ошибка SDL: %s", SDL_GetError());
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return SDLERR;
}

/*Функция рисования буквы 'C' рисовальщиком rend цвета r g b,
сегментами размера segment по координатам угла поля corner */
int DrawLetterC(SDL_Renderer* rend, const SDL_Rect* corner, int segment, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Rect rect = { corner->x + segment, corner->y + segment,
						segment * 4, segment * 3 };

	if (SDL_SetRenderDrawColor(rend, r, g, b, 0xFF) ||
		SDL_RenderFillRect(rend, &rect))
	{
		return SDLERR;
	}
	/*else*/
	return NOERR;
}

/*Функция рисования буквы 'E' рисовальщиком rend цвета r g b,
сегментами размера segment по координатам угла поля corner */
int DrawLetterE(SDL_Renderer* rend, const SDL_Rect* corner, int segment, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Rect rect1 = { corner->x + segment, corner->y + segment,
						segment * 3, segment };
	SDL_Rect rect2 = { corner->x + (segment * 2), corner->y + segment * 3,
						segment * 3, segment };

	if (SDL_SetRenderDrawColor(rend, r, g, b, 0xFF) ||
		SDL_RenderFillRect(rend, &rect1))
	{
		return SDLERR;
	}
	if (SDL_RenderFillRect(rend, &rect2))
	{
		return SDLERR;
	}
	/*else*/
	return NOERR;
}

/*Функция рисования буквы 'M' рисовальщиком rend цвета r g b,
сегментами размера segment по координатам угла поля corner */
int DrawLetterM(SDL_Renderer* rend, const SDL_Rect* corner, int segment, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Rect rect1 = { corner->x + segment, corner->y + segment,
						segment, segment * 4 };
	SDL_Rect rect2 = { corner->x + segment * 3, corner->y + segment,
						segment, segment * 4 };

	if (SDL_SetRenderDrawColor(rend, r, g, b, 0xFF) ||
		SDL_RenderFillRect(rend, &rect1))
	{
		return SDLERR;
	}
	if (SDL_RenderFillRect(rend, &rect2))
	{
		return SDLERR;
	}
	/*else*/
	return NOERR;
}

/*Функция рисования буквы 'H' рисовальщиком rend цвета r g b,
сегментами размера segment по координатам угла поля corner */
int DrawLetterH(SDL_Renderer* rend, const SDL_Rect* corner, int segment, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Rect rect1 = { corner->x + segment * 2, corner->y,
						segment, segment * 2 };
	SDL_Rect rect2 = { corner->x + segment * 2, corner->y + segment * 3,
						segment, segment * 2 };

	if (SDL_SetRenderDrawColor(rend, r, g, b, 0xFF) ||
		SDL_RenderFillRect(rend, &rect1))
	{
		return SDLERR;
	}
	if (SDL_RenderFillRect(rend, &rect2))
	{
		return SDLERR;
	}
	/*else*/
	return NOERR;
}

/*Функция рисования слова "СЕМЕН" рисовальщиком rend цвета r g b,
сегментами размера segment по координатам массива полей backRect */
int DrawLetters(SDL_Renderer* rend, const SDL_Rect* backRect, int segment, Uint8 r, Uint8 g, Uint8 b)
{
	if (DrawLetterC(rend, backRect, segment, r, g, b))
		return SDLERR;
	if (DrawLetterE(rend, backRect + 1, segment, r, g, b))
		return SDLERR;
	if (DrawLetterM(rend, backRect + 2, segment, r, g, b))
		return SDLERR;
	if (DrawLetterE(rend, backRect + 3, segment, r, g, b))
		return SDLERR;
	if (DrawLetterH(rend, backRect + 4, segment, r, g, b))
		return SDLERR;
	/*else*/
	return NOERR;
}

/*Получение размера сегмента*/
int GetSegmentSize(void)
{
	int num;
	printf("Слово состоит из пяти букв, каждая размеров 5x5 сегментов.\n");
	do
	{
		printf("Введите размер одного сегмента: ");
		scanf("%d", &num);
		if (num > 0)
			return num;
		/*else*/puts("Неправильный размер сегмента!");
	} while (TRUE);
}
