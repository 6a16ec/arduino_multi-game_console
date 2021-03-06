const int8_t Xaxis  = A2;                 // Определяем номер вывода, к которому подключен контакт оси Х джойстика
const int8_t Yaxis  = A1;                 // Определяем номер вывода, к которому подключен контакт оси У джойстика
const int8_t Button = 2;                  // Определяем номер вывода, к которому подключен контакт кнопки джойстика
uint16_t     XborderMIN = 490;            // Задаём границу значений, НИЖЕ которой будет считаться, что джойстик отклонён по оси Х влево
uint16_t     XborderMAX = 530;            // Задаём границу значений, ВЫШЕ которой будет считаться, что джойстик отклонён по оси Х вправо
uint16_t     YborderMIN = 490;            // Задаём границу значений, НИЖЕ которой будет считаться, что джойстик отклонён по оси У вниз
uint16_t     YborderMAX = 530;            // Задаём границу значений, ВЫШЕ которой будет считаться, что джойстик отклонён по оси У вверх
uint16_t     Xvol = 0, Yvol = 0;          // Задаём переменные, которые будут принимать значения, считанные с осей джойстика
uint16_t     last_time = 0;               // Задаём переменную для последнего времени перемещения


#include <iarduino_OLED.h>                                                            // Подключаем библиотеку iarduino_OLED.
iarduino_OLED myOLED(0x3C);                                                           // Объявляем объект myOLED, указывая адрес дисплея на шине I2C: 0x3C.
extern uint8_t MediumFontRus[];                                                       // Подключаем шрифт MediumFontRus.
extern uint8_t SmallFontRus[];                                                        // Подключаем шрифт SmallFontRus.
/* Объявление переменных  */
int Tail[115];                                                                        // Массив тела змейки.
int Choice, NextChoice;                                                               // Переменные выбора движения.
int nTail;                                                                            // Переменная количества элементов в змейке.
int CoreX;                                                                            // Переменная X-координаты квадратика.
int CoreY;                                                                            // Переменные Y-координаты квадратика.
int x;                                                                                // Переменная X-координаты движения.
int y;                                                                                // Переменная Y-координаты движения.
int EndX;                                                                             // Переменная X-координаты последнего элемента массива.
int EndY;                                                                             // Переменная Y-координаты последнего элемента массива.
int del;                                                                              // Переменная задержки.
int ycor;                                                                             // Дополнительная переменная Y-координаты с которой строится и выводится завершающаяся змейка.
int aTail;                                                                            // Дополнительная переменная количества элементов в змейке.
int8_t Pause;                                                                         // Переменная для хранения состояния паузы
/* Объявление функций  */
void ShowSnake();                                                                     // Функция вывода всей змейки на экран после выигрыша или проигрыша.
void SplashScreen();                                                                  // Функция вывода заставки игры на экран.
void GameOver();                                                                      // Функция завершения игры.
void SpeedControl();                                                                  // Функция регулировки скорости.
void MoveControl();                                                                   // Функция движения змейки и выбора направления.
//

void setup_snake(){
    SplashScreen();                                                                     // Функция вывода заставки игры на экран.

    nTail = 3;                                                                          // Назначаем количество элементов в змейке 3.
    del = 200;                                                                          // Присваиваем переменной значение задержки 200 мс.
    y = random(3, 30);                                                                  // Начальное положение Y-координаты.
    x = random(3, 14);                                                                  // Начальное положение X-координаты.
    Choice = random(1, 4);                                                              // Начальное направление движения.
    NextChoice = Choice;                                                                // Начальное направление движения.
    //
    for (int i = 0; i <= nTail-1; i++)                                                  // Формируем змейку из трех элементов в зависимости от направления движения. Присваиваем координаты каждому элементу змейке.
    {                                                                                 //
        Tail[i] = y;                                                                    // Присваиваем Y-координату элементу массива.
        Tail[i] = Tail[i] << 8;                                                         // Сдвигаем побитно на 8.
        Tail[i] += x;                                                                   // Присваиваем X-координату элементу массива.
        switch (Choice)                                                                 // В зависимости от направления движения формируем хвост в противоположную сторону.
        {                                                                             //
            case 1:                                                                     // Движение вверх.
                x++;                                                                      // Формируем хвост змейки вниз.
                break;                                                                    //
            case 2:                                                                     // Движение вниз.
                x--;                                                                      // Формируем хвост змейки вверх.
                break;                                                                    //
            case 3:                                                                     // Движение влево.
                y++;                                                                      // Формируем хвост змейки вправо.
                break;                                                                    //
            case 4:                                                                     // Движение вправо.
                y--;                                                                      // Формируем хвост змейки влево.
                break;                                                                    //
        }                                                                             //
    }                                                                                 //
    //
    for (int i = 0; i <= nTail-1; i++){                                                 // Выводим змейку на экран с помощью цикла.
        myOLED.drawRect(highByte(Tail[i])*4, lowByte(Tail[i])*4, highByte(Tail[i])*4+4, lowByte(Tail[i])*4+4, true, 1);}
    //
    CoreY = random(2, 32);                                                              // Задаем Y-координату положения квадратика.
    CoreX = random(2, 15);                                                              // Задаем X-координату положения квадратика.
    myOLED.drawRect (CoreY*4, CoreX*4,  CoreY*4 + 4, CoreX*4 + 4, true , 1);            // Выводим квадратик.
    //
    y = highByte(Tail[0]);                                                              // Y-координату первого элемента массива присваиваем начальной Y-координате движения.
    x = lowByte(Tail[0]);                                                               // X-координату первого элемента массива присваиваем начальной X-координате движения.
}

void setup()
{
    pinMode(Button, INPUT);                                                             // Настраиваем вывод Button на работу в режиме входа
    myOLED.begin();                                                                     // Инициируем работу с дисплеем.
    myOLED.autoUpdate(false);                                                           // Запрещаем автоматический вывод данных. Информация на дисплее будет обновляться только после обращения к функции update().
    myOLED.clrScr();                                                                    // Чистим экран.
    while(!Serial){}                                                                    // Ждём готовность Serial к передаче данных в монитор последовательного порта.
    //
    Pause = 0;
    setup_snake()
}

void loop_snake(){
    if(Pause == 0)                                                                      // Если пауза не включена
    {
        Xvol = analogRead(Xaxis);                                                         // Считываем значения оси Х
        Yvol = analogRead(Yaxis);                                                         // Считываем значения оси У
        MoveControl();                                                                    // Функция движения змейки и выбора направления.
    }

    if (digitalRead(Button)) {                                                          // Проверяем, нажата ли кнопка
        delay(5);
        while (digitalRead(Button)) {}                                                    // Если кнопка удерживается, то ничего не делаем
        delay(5);                                                                        // Если кнопка отпускается, то подавляем дребезг
        if(Pause == 0) {
            Pause = 1;                                                                      // Выводим текст по центру 20 строки.} .
            myOLED.print("ПАУЗА", OLED_C, 40);
            myOLED.update();
        }                                                                                 // Если не было паузы, ставим
        else {
            Pause = 0;
            myOLED.clrScr();
            for (int i = 0; i <= nTail-1; i++){                                             // Выводим змейку на экран с помощью цикла.
                myOLED.drawRect(highByte(Tail[i])*4, lowByte(Tail[i])*4, highByte(Tail[i])*4+4, lowByte(Tail[i])*4+4, true, 1);}
            myOLED.drawRect (CoreY*4, CoreX*4,  CoreY*4 + 4, CoreX*4 + 4, true , 1);        // Выводим квадратик

        }                                                                               // Если была, убираем
    }
    //
    if(millis() - last_time > del && Pause == 0){                                       // Регулируем скорость и учитываем паузу
        last_time = millis();                                                               // Обновляем последнее время перемещения
        SpeedControl();                                                                     // Функция регулировки скорости.
        if(Choice == 1) x -= 1;
        if(Choice == 2) x += 1;
        if(Choice == 3) y -= 1;
        if(Choice == 4) y += 1;
        Choice = NextChoice;                                                                // Подтверждаем выбор направления движения
        //
        myOLED.drawRect   (127, 63,  0, 0, false , 1);                                      // Обновляем каждый шаг рамку игрового поля.
        myOLED.drawRect (CoreY*4, CoreX*4,  CoreY*4 + 4, CoreX*4 + 4, true , 1);            // Обновляем каждый шаг квадратик.
        //
        EndY = highByte(Tail[nTail-1]);                                                     // Запоминаем Y-координаты последнего элемента массива.
        EndX = lowByte(Tail[nTail-1]);                                                      // Запоминаем Y-координаты последнего элемента массива.
        //
        //
        int PrevSY = highByte(Tail[0]);                                                     // Дополнительная переменная для переноса Y-координаты массива.
        int PrevSX = lowByte(Tail[0]);                                                      // Дополнительная переменная для переноса X-координаты массива.
        int Prev2SX, Prev2SY;                                                               // Вторые дополнительные переменные для переноса координат массива.
        Tail[0] = y;                                                                        // Присваиваем новую Y-координату первому элементу массива.
        Tail[0] = Tail[0] << 8;                                                             // Сдвигаем побитно на 8.
        Tail[0] += x;                                                                       // Присваиваем новую Y-координату первому элементу массива.
        for (int i = 1; i <= nTail-1; i++)                                                  // Переносим каждый элемент массива по очереди с помощью цикла.
        {                                                                                   //
            Prev2SY = highByte(Tail[i]);                                                      // Присваиваем второй дополнительное переменной Y-координату массива.
            Prev2SX = lowByte(Tail[i]);                                                       // Присваиваем второй дополнительное переменной X-координату массива.
            Tail[i] = PrevSY;                                                                 // Присваиваем новому элементу массива старую Y-координату предыдущего элемента массива.
            Tail[i] = Tail[i] << 8;                                                           // Сдвигаем побитно на 8.
            Tail[i] += PrevSX;                                                                // Присваиваем новому элементу массива старую X-координату предыдущего элемента массива.
            PrevSY = Prev2SY;                                                                 // Присваиваем дополнительным переменным с Y-координатой вторые дополнительные переменные с Y-координатой.
            PrevSX = Prev2SX;                                                                 // Присваиваем дополнительным переменным с X-координатой вторые дополнительные переменные с X-координатой.
        }                                                                                   //
        //
        for (int i = 0; i <= nTail-1; i++){                                                 // Выводим змейку на экран.
            myOLED.drawRect   (highByte(Tail[i])*4, lowByte(Tail[i])*4,  highByte(Tail[i])*4 + 4, lowByte(Tail[i])*4 + 4, true , 1);}
        //
        myOLED.drawRect   (EndY*4, EndX*4,  EndY*4 + 4, EndX*4 + 4, true , 0);              // Очищаем последний элемент змейки.
        myOLED.update();                                                                    // Обновляем информацию на дисплее.
        //
        if (x == CoreX && y == CoreY)                                                       // Сравниваем координаты змейки с координатами квадратика.
        {                                                                                   //
            myOLED.drawRect (CoreY*4, CoreX*4,  CoreY*4 + 4, CoreX*4 + 4, true , 0);          // Если они совпадают, очищаем старый квадратик.
            CoreY = random(2, 32);                                                            // Задаем новую Y-координату квадратика.
            CoreX = random(2, 15);                                                            // Задаем новую X-координату квадратика.
            nTail++;                                                                          // Увеличиваем количество элементов в змейке на один.
        }                                                                                   //
        //
        if (x > 14 || x < 0 || y > 31 || y < 0){                                            // Проверка выхода змейки за пределы игрового поля. Если координат движения больше, чем игровое поле, то игра заканчивается.
            if(x > 14) x = 0;
            if(x < 0) x = 14;
            if(y > 31) y = 0;
            if(y < 0) y = 31;
        }                                                                                   // Функция вывода всей змейки на экран после проигрыша.
        //
        for (int i = 1; i < nTail-1; i++){                                                  // Проверки столкновения головы змейки с хвостом.
            if (y == highByte(Tail[i]) && x == lowByte(Tail[i])){                               // С помощью цикла проверяем каждый элемент массива с координатами движения. Если они совпадают, то голова столкнулась с хвостом.
                GameOver();                                                                         // Функция завершения игры.
                ShowSnake();}}                                                                      // Функция вывода всей змейки на экран после проигрыша.
        //
        if (nTail == 115){                                                                  // Проверка количества элементов змейки на выигрыш.
            myOLED.clrScr();                                                                    // Чистим экран.
            myOLED.setFont(MediumFontRus);                                                      // Указываем шрифт который требуется использовать для вывода цифр и текста.
            myOLED.print("ВЫИГРЫШ!", OLED_C, 20);                                               // Выводим текст по центру 30 строки.
            myOLED.autoUpdate(true);                                                            // Разрешаем автоматический вывод данных.
            ShowSnake();}                                                                       // Функция вывода всей змейки на экран после выигрыша.
    }
}

void loop()
{
    loop_snake()
}
// Функция вывода всей змейки на экран после выигрыша или проигрыша.
void ShowSnake()                                                                      //
{                                                                                     //
    ycor = 6;                                                                           // Присваиваем дополнительное координате значение 6. С 6 Y-координаты строим змейку столбцами по 5 элементов.
    for (int k = 1; k <= 100; k++)                                                      // Цикл вывода змейки на экран.
    {                                                                                   //
        if (nTail > 5)                                                                    // Сравниваем количество элементов с 5. Столбцы формируемые снизу вверх. Нечетные столбцы.
        {                                                                                 // Если больше 5.
            aTail = 5;                                                                      // Присваиваем дополнительной переменной значение 5.
            for (int i = 56; i >= 56 - aTail*4; i = i - 5)                                  // Цикл формирования и вывода змейки по координатам, начиная с X = 56 до X = 36 с шагом 5.
            {                                                                               //
                myOLED.drawRect (ycor, i, ycor+3, i+3, true, 1);                              // Вывод змейки квадратами начиная с координат Y = 6, X = 56, Y = 9, X = 59 до координат Y = 6, X = 36 Y = 9, X = 39. Значения Y-координаты указаны для первого столбца.
                delay(200);                                                                   // Задержка 200 мс.
            }                                                                               //
            nTail = nTail - 5;                                                              // Вычитаем из количества элементов в змейке 5. Для определения количества выводимых столбцов.
        }                                                                                 //
        else                                                                              // Если меньше 5.
        {                                                                                 //
            aTail = nTail;                                                                  // Присваиваем оставшееся количество элементов дополнительной переменной.
            for (int i = 56; i >= 56 - aTail*4; i = i - 5)                                  // Цикл формирования и вывода змейки по координатам с шагом 5, начиная с X = 56 до X-координаты соответствующая значению количества оставшихся элементов змейки.
            {                                                                               //
                myOLED.drawRect (ycor, i, ycor+3, i+3, true, 1);                              // Вывод змейки квадратами начиная с координат Y = 6, X = 56, Y = 9, X = 59 до координат Y = 6, X = 36 Y = 9, X = 39. Значения Y-координаты указаны для первого столбца.
                delay(200);                                                                   // Задержка 200 мс.
            }                                                                               //
            delay (4000);                                                                   // Задержка 4 секунды.
            return setup();                                                                 // Возвращаемся в функцию Setup().
        }                                                                                 //
        if (nTail > 5)                                                                    // Сравниваем количество элементов с 5. Столбцы формируемые сверху вниз. Четные столбцы.
        {                                                                                 // Если больше 5.
            aTail = 5;                                                                      // Присваиваем дополнительной переменной значение 5.
            for (int i = 36; i <= 36 + aTail*4; i = i + 5)                                  // Цикл формирования и вывода змейки по координатам, начиная с X = 36 до X = 56 с шагом 5.
            {                                                                               //
                myOLED.drawRect (ycor+5, i, ycor+8, i+3, true, 1);                            // Вывод змейки квадратами начиная с координат Y = 11, X = 36, Y = 14, X = 39 до координат Y = 11, X = 56 Y = 14, X = 59. Значения Y-координаты указаны для первого столбца.
                delay(200);                                                                   // Задержка 200 мс.
            }                                                                               //
            nTail = nTail - 5;                                                              // Вычитаем из количества элементов в змейке 5. Для определения количества выводимых столбцов.
        }                                                                                 //
        else                                                                              // Если меньше 5.
        {                                                                                 //
            aTail = nTail;                                                                  // Присваиваем оставшееся количество элементов дополнительной переменной.
            for (int i = 36; i <= 36 + aTail*4; i = i + 5)                                  // Цикл формирования и вывода змейки по координатам с шагом 5, начиная с X = 36 до X-координаты соответствующая значению количества оставшихся элементов змейки.
            {                                                                               //
                myOLED.drawRect (ycor+5, i, ycor+8, i+3, true, 1);                            // Вывод змейки квадратами начиная с координат Y = 11, X = 36, Y = 14, X = 39 до координат Y = 11, X = 56 Y = 14, X = 59. Значения Y-координаты указаны для первого столбца.
                delay(200);                                                                   // Задержка 200 мс.
            }                                                                               //
            delay (4000);                                                                   // Задержка 4 секунды.
            return setup();                                                                 // Возвращаемся в функцию Setup().
        }                                                                                 //
        ycor = ycor + 10;                                                                 // Увеличиваем дополнительную Y-координату на значение 10. Строим новый ряд столбцов.
    }                                                                                   //
}                                                                                     //
// Функция вывода заставки игры на экран.
void SplashScreen()                                                                   //
{                                                                                     //
    myOLED.setFont(MediumFontRus);                                                      // Указываем шрифт который требуется использовать для вывода цифр и текста.
    myOLED.print("Змейка", OLED_C, 27);                                                 // Выводим текст по центру 30 строки.
    myOLED.drawRect   (127, 63,  0, 0, false , 1);                                      // Выводим рамку игрового поля.
    // Формируем рисунок змейки с помощью циклов и выводим на экран.
    for (int i = 56; i >= 36; i = i - 5){                                               // Цикл формирования рисунка змейки.                    //
        myOLED.drawRect   (16, i,  19, i + 3, true , 1);                                    // Выводим рисунок на экран.                            //
        delay(300);}                                                                        // Задержка 300 мс.                                     //
    for (int i = 21; i <= 41; i = i + 5){                                               // Цикл формирования рисунка змейки.                    //
        myOLED.drawRect   (i, 36,  i + 3, 39, true , 1);                                    // Выводим рисунок на экран.                            //
        delay(300);}                                                                        // Задержка 300 мс.                                     //
    for (int i = 41; i <= 51; i = i + 5){                                               // Цикл формирования рисунка змейки.                    //        # # # # #
        myOLED.drawRect   (41, i,  44, i + 3, true , 1);                                    // Выводим рисунок на экран.                            //        #       #     # # #
        delay(300);}                                                                        // Задержка 300 мс.                                     //        #       # # # #
    for (int i = 46; i <= 56; i = i + 5){                                               // Цикл формирования рисунка змейки.                    //        #
        myOLED.drawRect   (i, 51,  i + 3, 54, true , 1);                                    // Выводим рисунок на экран.                            //        #
        delay(300);}                                                                        // Задержка 300 мс.                                     //
    myOLED.drawRect   (56, 46,  59, 49, true , 1);                                      // Выводим рисунок на экран.                            //
    delay(300);                                                                         // Задержка 300 мс.                                     //
    for (int i = 61; i <= 66; i = i + 5){                                               // Цикл формирования рисунка змейки.                    //
        myOLED.drawRect   (i, 46,  i + 3, 49, true , 1);                                    // Выводим рисунок на экран.                            //
        delay(300);}                                                                        // Задержка 300 мс.                                     //
    for (int i = 0; i <= 5; i++){                                                       // Рисунок мигающего элемента. С помощью цикла формируем мигание.
        myOLED.drawRect   (91, 46,  94, 49, true , 0);                                      // Гасим рисунок.
        delay(300);                                                                         // Задержка 300 мс.
        myOLED.drawRect   (91, 46,  94, 49, true , 1);                                      // Выводим рисунок.
        delay(300);}                                                                        // Задержка 300 мс.
    //
    myOLED.clrScr();                                                                    // Чистим экран.
    myOLED.print("От", OLED_C, 25);                                                     // Выводим текст по центру 20 строки.
    myOLED.print("Шутков Н.А.", OLED_C, 50);                                               // Выводим текст по центру 20 строки.
    delay (3000);                                                                       // Задержка 3 секунды.
}                                                                                     //
// Функция завершения игры.
void GameOver()                                                                       //
{                                                                                     //
    myOLED.clrScr();                                                                    // Чистим экран.
    myOLED.setFont(SmallFontRus);                                                       // Указываем шрифт который требуется использовать для вывода цифр и текста.
    myOLED.print("Конец игры", OLED_C, 14);                                             // Выводим текст по центру 25 строки.
    myOLED.print("Результат - ", 25, 25);                                               // Выводим текст 25 столбца 20 строки.
    myOLED.print(nTail-3, 95, 25);                                                      // Выводим переменную 95 столбца 40 строки.
    myOLED.autoUpdate(true);                                                            // Разрешаем автоматический вывод данных.
}                                                                                     //
// Функция регулировки скорости.
void SpeedControl()                                                                   //
{                                                                                     //
    if (nTail == 5){del = 100;}                                                         // Вторая скорость. Задержка 100 мс. Количество элементов змейки = 5.
    if (nTail == 10){del = 50;}                                                         // Третья скорость. Задержка 50 мс. Количество элементов змейки = 10.
    if (nTail == 15){del = 0;}                                                          // Четвертая скорость. Задержки нет. Количество элементов змейки = 15.
}                                                                                     //
// Функция движения змейки и выбора направления.
void MoveControl()                                                                    //
{                                                                                     //
    switch (Choice)                                                                     //
    {                                                                                 //
        case 3:                                                                         // Движение влево.
            if (Yvol > YborderMAX){NextChoice = 1;}                                       // Проверяем данные по оси Y. Поворот вверх.
            if (Yvol < YborderMIN){NextChoice = 2;}                                       // Проверяем данные по оси Y. Поворот вниз.
            // y--;                                                                          // Координаты формирования змейки при движении влево. Уменьшаем координату Y на количество пройденных квадратиков.
            break;                                                                        //
            //
        case 4:                                                                         // Движение вправо.
            if (Yvol > YborderMAX){NextChoice = 1;}                                       // Проверяем данные по оси Y. Поворот вверх.
            if (Yvol < YborderMIN){NextChoice = 2;}                                       // Проверяем данные по оси Y. Поворот вниз.
            // y++;                                                                          // Координаты формирования змейки при движении влево. Увеличиваем координату Y на количество пройденных квадратиков.
            break;                                                                        //
            //
        case 2:                                                                         // Движение вниз.
            if (Xvol > XborderMAX){NextChoice = 4;}                                       // Проверяем данные по оси X. Поворот вправо.
            if (Xvol < XborderMIN){NextChoice = 3;}                                       // Проверяем данные по оси X. Поворот влево.
            // x++;                                                                          // Координаты формирования змейки при движении влево. Увеличиваем координату X на количество пройденных квадратиков.
            break;                                                                        //
            //
        case 1:                                                                         // Движение вверх.
            if (Xvol > XborderMAX){NextChoice = 4;}                                       // Проверяем данные по оси X. Поворот вправо.
            if (Xvol < XborderMIN){NextChoice = 3;}                                       // Проверяем данные по оси X. Поворот влево.
            // x--;                                                                          // Координаты формирования змейки при движении влево. Уменьшаем координату X на количество пройденных квадратиков.
            break;                                                                        //
    }                                                                                 //
}                                                                                     //
