#ifndef gdbms01_h
#define gdbms01_h

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>

#define BASEEXPORTFUNC _stdcall

typedef int TREE;
typedef short int BASE_INT;
typedef int BASE_INT64;

//== Список функций библиотеки GDBMS01.DLL
//   Помеченные символом * можно использовать в Visual C
typedef struct
{
 unsigned BaseOpen : 1;                                 // 001*
 unsigned BaseReadObject : 1;                           // 002*
 unsigned BaseOpen32 : 1;                               // 003
 unsigned BaseObjectConvertCoord : 1;                   // 004*
 unsigned BDelObjectInfo : 1;                           // 005
 unsigned BaseDeleteServiceObjects : 1;                 // 006*
 unsigned BaseObjectTextToIF : 1;                       // 007
 unsigned BaseObjectTextToIFOpen : 1;                   // 008
 unsigned BaseObjectTextToIFClose : 1;                  // 009
 unsigned BaseObjectTextToIFCont : 1;                   // 010
 unsigned BaseSetForTextToIF : 1;                       // 011
 unsigned BaseInitQuery : 1;                            // 012*
 unsigned BaseInitObject : 1;                           // 013*
 unsigned BaseClose : 1;                                // 014*
 unsigned BaseCloseObject : 1;                          // 015*
 unsigned BaseOpenTextError : 1;                        // 016*
 unsigned BaseSetFormat : 1;                            // 017*
 unsigned BaseGetFormat : 1;                            // 018*
 unsigned BaseSetMetDescriptor : 1;                     // 019*
 unsigned BasePackMode : 1;                             // 020*
 unsigned BaseReadObjectDown : 1;                       // 021
 unsigned BaseSizeObject : 1;                           // 022
 unsigned BaseTreeNavObject : 1;                        // 023
 unsigned BaseTreeNavCode : 1;                          // 024
 unsigned BaseReadNumbers : 1;                          // 025*
 unsigned BaseSetupQuery : 1;                           // 026*
 unsigned BaseReadConnectObjects : 1;                   // 027*
 unsigned BaseReadNameClass : 1;                        // 028*
 unsigned BaseReadName : 1;                             // 029*
 unsigned BaseReadModel : 1;                            // 030
 unsigned BaseReadListkv : 1;                           // 031*
 unsigned BaseReadListkv1 : 1;                          // 032
 unsigned BaseReadPassword : 1;                         // 033*
 unsigned BaseObjectStat : 1;                           // 034
 unsigned BaseReadObjectList : 1;                       // 035*
 unsigned BaseReadCodeList : 1;                         // 036
 unsigned BaseReadObjectKV : 1;                         // 037*
 unsigned BaseReadObjectLKV : 1;                        // 038*
 unsigned BaseGetSemMinMaxValue : 1;                    // 039
 unsigned BaseReadObjectListSem : 1;                    // 040
 unsigned BaseReadClassInfo : 1;                        // 041*
 unsigned BaseGetVersion : 1;                           // 042
 unsigned BaseCheckLists : 1;                           // 043
 unsigned BaseGetFreeNum : 1;                           // 044*
 unsigned BaseObjectCount : 1;                          // 045*
 unsigned BaseInitMultiQuery : 1;                       // 046*
 unsigned BaseMultiReadObjectOpen : 1;                  // 047*
 unsigned BaseMultiReadObjectCont : 1;                  // 048*
 unsigned BaseMultiReadObjectRetry : 1;                 // 049*
 unsigned BaseMultiReadObjectRestore : 1;               // 050*
 unsigned BaseMultiReadObjectListOpen : 1;              // 051*
 unsigned BaseMultiReadObjectListCont : 1;              // 052*
 unsigned BaseMultiReadObjectListRetry : 1;             // 053*
 unsigned BaseMultiReadObjectClose : 1;                 // 054*
 unsigned BaseCheckObject : 1;                          // 055
 unsigned BaseWriteObject : 1;                          // 056*
 unsigned BaseDeleteObject : 1;                         // 057*
 unsigned BaseWriteNameClass : 1;                       // 058*
 unsigned BaseWriteName : 1;                            // 059*
 unsigned BaseSetFindPrecision : 1;                     // 060
 unsigned BaseGetFindPrecision : 1;                     // 061
 unsigned BaseGetErrorMessage : 1;                      // 062
 unsigned BaseWriteModel : 1;                           // 063
 unsigned BaseWriteListkv : 1;                          // 064*
 unsigned BaseWriteListkv1 : 1;                         // 065
 unsigned BaseCheckPoint : 1;                           // 066*
 unsigned BaseCreateListMKO : 1;                        // 067
 unsigned BaseCreateListSem : 1;                        // 068
 unsigned BaseDeleteList : 1;                           // 069
 unsigned BaseDeleteListItem : 1;                       // 070
 unsigned BaseDeleteListCodeNum : 1;                    // 071
 unsigned BaseWriteListItem : 1;                        // 072
 unsigned BaseWriteListItemOpt : 1;                     // 073
 unsigned BaseDeleteLKV : 1;                            // 074
 unsigned BaseMultiMediaItemAdd : 1;                    // 075
 unsigned BaseMultiMediaItemDel : 1;                    // 076
 unsigned BaseMultiMediaItemNo : 1;                     // 077
 unsigned BaseGetMultiMediaItem : 1;                    // 078
 unsigned BaseMultiMediaItemDsc : 1;                    // 079
 unsigned BaseReadSvPr : 1;                             // 080*
 unsigned BaseGetAdrSvPrForNumber : 1;                  // 081*
 unsigned BaseDeleteObjectSvPr : 1;                     // 082
 unsigned BaseDeleteSvPr : 1;                           // 083*
 unsigned BaseAddSvPr : 1;                              // 084*
 unsigned BaseAddPrToPoint : 1;                         // 085*
 unsigned BaseDeletePrNumber : 1;                       // 086*
 unsigned BaseGetAdrSemSP : 1;                          // 087*
 unsigned BaseCoordConvert : 1;                         // 088
 unsigned BaseCodeStr : 1;                              // 089*
 unsigned BaseTransMKO : 1;                             // 090*
 unsigned BaseNumbersCharToInt : 1;                     // 091*
 unsigned BaseNumbersCharToLong : 1;                    // 092
 unsigned BaseNumbersCharToLongZ : 1;                   // 093
 unsigned BaseDateEncode : 1;                           // 094*
 unsigned BaseDateDecode : 1;                           // 095*
 unsigned BaseObjectSquare : 1;                         // 096
 unsigned BaseObjectSquare4 : 1;                        // 097
 unsigned BaseObjectSquareAll : 1;                      // 098*
 unsigned BasePrintObjectFile : 1;                      // 099
 unsigned BasePrintObjectToFile : 1;                    // 100
 unsigned BasePointInout : 1;                           // 101
 unsigned BaseCopyObject : 1;                           // 102*
 unsigned BaseCompCode : 1;                             // 103*
 unsigned BaseGetError : 1;                             // 104
 unsigned BaseConvFloatLong : 1;                        // 105
 unsigned BaseCoordIntToLong : 1;                       // 106
 unsigned BaseSelectObject : 1;                         // 107
 unsigned BaseMultiSelectObject : 1;                    // 108
 unsigned BaseMultiSelectCodeNumber : 1;                // 109
 unsigned BaseMultiSelectOneSP : 1;                     // 110
 unsigned BaseFormKV : 1;                               // 111
 unsigned BaseGetMemory : 1;                            // 112
 unsigned BaseFreeMemory : 1;                           // 113
 unsigned BaseReadSem : 1;                              // 114
 unsigned BaseReadSemFromNumber : 1;                    // 115
 unsigned BaseAddOneSem : 1;                            // 116
 unsigned BaseWriteOneSem : 1;                          // 117
 unsigned BaseDeleteOneSem : 1;                         // 118
 unsigned BaseFormSem : 1;                              // 119
 unsigned BaseReadSemComp : 1;                          // 120
 unsigned BaseSemCount : 1;                             // 121*
 unsigned BaseFeatureSplit : 1;                         // 122
 unsigned BaseSemLength : 1;                            // 123
 unsigned BaseSemValueUpdate : 1;                       // 124
 unsigned BaseReadSTRX11 : 1;                           // 125
 unsigned BaseGetX11SingleFieldCode : 1;                // 126
 unsigned BaseGetX11SingleFieldNumb : 1;                // 127
 unsigned BasePrintSemValue : 1;                        // 128
 unsigned BasePrintSemValue5 : 1;                       // 129
 unsigned BaseReadSemValue : 1;                         // 130
 unsigned BaseReadFeature : 1;                          // 131*
 unsigned BaseReadFeatureComp : 1;                      // 132*
 unsigned BaseDeleteFeature : 1;                        // 133*
 unsigned BaseDeleteFeatureComp : 1;                    // 134*
 unsigned BaseAddFeature : 1;                           // 135*
 unsigned BaseAddFeatureComp : 1;                       // 136*
 unsigned BaseUpdateFeature : 1;                        // 137*
 unsigned BaseUpdateFeatureComp : 1;                    // 138*
 unsigned BaseWriteImageFile : 1;                       // 139
 unsigned BaseWriteImageMemory : 1;                     // 140
 unsigned BaseDeleteImage : 1;                          // 141
 unsigned BaseObjectDeleteImage : 1;                    // 142
 unsigned BaseReadImageFile : 1;                        // 143
 unsigned DDCReadImageFile : 1;                         // 144
 unsigned BaseReadImageMemory : 1;                      // 145
 unsigned BaseObjectReadImageFile : 1;                  // 146
 unsigned BaseObjectReadImageMemory : 1;                // 147
 unsigned BaseSetAdrSem : 1;                            // 148
 unsigned BaseSetAdrSv : 1;                             // 149
 unsigned BaseSetAdrPr : 1;                             // 150
 unsigned BaseSetAdrSquare : 1;                         // 151
 unsigned BaseSetAdrMet : 1;                            // 152
 unsigned BaseSetAdrVideo : 1;                          // 153
 unsigned BaseSetAdrText : 1;                           // 154
 unsigned BaseFindObjectByCoordInt : 1;                 // 155
 unsigned BaseFindObjectByCoordFloat : 1;               // 156
 unsigned BaseFindDOByCoordInt : 1;                     // 157
 unsigned BaseFindAllDOByCoordInt : 1;                  // 158
 unsigned BaseFindAllDOByCoordFloat : 1;                // 159
 unsigned BaseFindDOByCoordFloat : 1;                   // 160
 unsigned BaseFindObjectStreamInt : 1;                  // 161
 unsigned BaseFindObjectStreamFloat : 1;                // 162
 unsigned BaseFindAnyObjectCoord : 1;                   // 163
 unsigned BaseFindAnyObjectCoordIF : 1;                 // 164*
 unsigned BaseMultiFindAnyObjectCoord : 1;              // 165
 unsigned BaseMultiFindAnyObjectCoordIF : 1;            // 166*
 unsigned BaseScaleAnalyze : 1;                         // 167
 unsigned BaseGetLabel : 1;                             // 168
 unsigned BaseSetFindMaxDistance : 1;                   // 169*
 unsigned BaseGetFindMaxDistance : 1;                   // 170*
 unsigned DDCOpen : 1;                                  // 171
 unsigned DDCClose : 1;                                 // 172
 unsigned DDCReadNameClass : 1;                         // 173
 unsigned DDCReadName : 1;                              // 174
 unsigned DDCReadObject : 1;                            // 175
 unsigned DDCInitQuery : 1;                             // 176
 unsigned DDCReadHeader : 1;                            // 177
 unsigned DDCFindObjectByCoordInt : 1;                  // 178
 unsigned DDCFindObjectByCoordFloat : 1;                // 179
 unsigned DDCFindAllDOByCoordInt : 1;                   // 180
 unsigned DDCFindAllDOByCoordFloat : 1;                 // 181
 unsigned BaseReadGlobal : 1;                           // 182
 unsigned BaseReadGlobalByPart : 1;                     // 183
 unsigned BaseReadClassInfoObject : 1;                  // 184*
 unsigned BaseMemoryMove : 1;                           // 185
 unsigned BasePackCode : 1;                             // 186
 unsigned BaseSetFlagCheckPr : 1;                       // 187
 unsigned TreeSK : 1;                                   // 188
 unsigned TreeS : 1;                                    // 189
 unsigned TreeCPY : 1;                                  // 190
 unsigned TreeSKD : 1;                                  // 191
 unsigned TreeSD : 1;                                   // 192
 unsigned a2daPrm : 1;                                  // 193
 unsigned BaseCalcSize : 1;                             // 194
 unsigned BaseCheckClassCode : 1;                       // 195
 unsigned BaseCopy : 1;                                 // 196
 unsigned BaseSetFeatureFilterRead : 1;                 // 197
 unsigned BaseSetFeatureFilterNoRead : 1;               // 198
 unsigned BaseGetFeatureFilterReadCount : 1;            // 199
 unsigned BaseGetFeatureFilterNoReadCount : 1;          // 200
 unsigned BaseGetFeatureFilterRead : 1;                 // 201
 unsigned BaseGetFeatureFilterNoRead : 1;               // 202
 unsigned BaseSetFeatureFilterService : 1;              // 203
 unsigned BaseGetFeatureFilterService : 1;              // 204
 unsigned BaseGetFeatureFilterServiceCount : 1;         // 205
 unsigned BaseSwapSvPr : 1;                             // 206
 unsigned BaseSortPr : 1;                               // 207
 unsigned BaseCreateListWindows : 1;                    // 208
 unsigned BaseSetMaxCodePositions : 1;                  // 209
 unsigned BaseGetMaxCodePositions : 1;                  // 210
 unsigned BaseChangeCodeObject : 1;                     // 211
 unsigned BaseChangeCodeInList : 1;                     // 212
 unsigned BaseAddMetric :1;                             // 213*
 unsigned BaseDelMetric :1;                             // 214*
 unsigned BaseUpdateMetric :1;                          // 215*
 unsigned BaseGetFreeNumbers :1;                        // 216*
 unsigned BaseFeatureDelFull :1;                        // 217
 unsigned BaseExtractDO :1;                             // 218
 unsigned BaseCorrRelations :1;                         // 219
 unsigned BaseCreateListWinForRegion :1;                // 220
 unsigned BaseGetSelectInfo :1;                         // 221
 unsigned BaseObjectLengthAll :1;                       // 222
 unsigned BasePointInoutEps : 1;                        // 223
 unsigned BaseCheckObjectFull : 1;                      // 224
 unsigned TreeOpen : 1;                                 // 225
 unsigned TreeClose : 1;                                // 226
 unsigned BaseReadObjectSV : 1;                         // 227
 unsigned BaseDeleteObjectSV : 1;                       // 228
 unsigned BaseWriteApplicationInfo : 1;                 // 229
 unsigned BaseWriteUserInfo : 1;                        // 230
 unsigned BaseSetCodeSemPoint : 1;                      // 231
 unsigned BaseGetCodeSemPoint : 1;                      // 232
 unsigned BaseGetNumberQuery : 1;                       // 233
 unsigned BaseCopyObjectEasy : 1;                       // 234
 unsigned BaseWriteObjectSV : 1;                        // 235
 unsigned BaseDeleteObjects : 1;                        // 236
 unsigned BaseCalcSizeDouble : 1;                       // 237
 unsigned BaseNumbersCharToDouble : 1;                  // 238
 unsigned BaseAddFeatureCompUniversal : 1;              // 239
 unsigned BaseCreateListWindowsByCoords : 1;            // 240
 unsigned BaseSetCoeffCoordGeo : 1;                     // 241
} GBASE_FUN01;


//--------------- Для работы с элементами мультимедиа -----------------
#define MULTI_BASE          1  // Элемент мультимедиа хранится в объекте базы
#define MULTI_FILE          0  // Элемент мультимедиа хранится в файле
#define MULTI_NO            0  // Не обрабатывать элемент мультимедиа
#define MULTI_DEL           1  // Удалить элемент
#define MULTI_ADD           2  // Добавить элемент
#define MULTI_DSC           3  // Изменить описание элемента

//--------------- Коды ошибок ----------------------------------------
#define BERR_OK             0  // Нормальное завершение
#define BERR_MEM            1  // Нет оперативной памяти
#define BERR_NOINF          2  // Нет информации по запросу
#define BERR_QUERY          3  // Ошибка в запросе
#define BERR_PARAM          4  // Ошибка в параметрах
#define BERR_PASS           5  // Неверный пароль
#define BERR_READ           6  // База открыта только на чтение
#define BERR_FILEBASE       7  // Файл не является базой данных
#define BERR_BASE           8  // Ошибка в базе данных
#define BERR_COORD          9  // Неверные координаты
#define BERR_WAIT          10  // База данных занята другой задачей
#define BERR_NOCLOSE       11  // База в незавершенном состоянии
#define BERR_BLOCK         12  // База заблокирована
#define BERR_FORMAT        13  // Неверный формат базы данных
#define BERR_YESINF        14  // Информация уже есть
#define BERR_NUMBER        15  // Ошибка в номере объекта
#define BERR_METRIC        16  // Формат метрики не поддерживается
#define BERR_SLMKO         17  // Ошибка в исходном тексте фильтра
#define BERR_RLMKO         18  // Ошибка в коде фильтра
#define BERR_CANCEL        19  // Отказ
#define BERR_FILE          20  // Ошибка в файле
#define BERR_MAX           21  // Количество элементов превышает максимально допустимое
#define BERR_REFOBJ        22  // В базе нет объекта по ссылке
#define BERR_OBJECT        23  // Ошибка в объекте в памяти
#define BERR_RO            24  // Ошибка в структуре распределенного объекта
#define BERR_DISK          25  // Недостаточно места на диске

//------- Расширенные коды ошибок -------------------------------
#define BASE_ERR_NUMBER    1
#define BASE_ERR_CODE      2
#define BASE_ERR_NAME      3
#define BASE_ERR_EXTFILE   4
#define BASE_ERR_FILE      5
#define BASE_ERR_FORMAT    6

//--------- Добавок к расширенному коду ошибки
#define  BASE_ERR_READ     1000       // Ошибки чтения из базы
#define  BASE_ERR_WRITE    2000       // Ошибки записи в базу
#define  BASE_ERR_DELETE   3000       // Ошибки при удалении из базы
#define  BASE_ERR_NIKA     4000       // Ошибки НИКИ

//-------  Операции отношения для BaseReadObjectListSem ---------
#define  BAL 0     //  Наличие
#define  BEQ 1     //  Равно
#define  BNE 2     //  Не равно
#define  BLE 3     //  Меньше или равно
#define  BLT 4     //  Меньше
#define  BGE 5     //  Больше или равно
#define  BGT 6     //  Больше
#define  BIN 7     //  В интервале
#define  BOU 8     //  Вне интервала

//----------- Для удаления объектов по BDelObjectInfo
#define DSubTree    2    // удаление дерева объектов
#define DCode       3    // удаление объектов указанного полного кода

//-------- Формат базы данных -------------------------------------
//         Действует только при записи в базу
#define    BASE_FORMAT_2     2       // Формат 2.0 - формат DOS (АРМ РАСТР-2)
#define    BASE_FORMAT_3     3       // Формат 3.1 - формат Windows

//---------- Для управления режимом упаковки базы данных ---------------
#define    BASE_NOPACK       0     // Не упаковывать
#define    BASE_PACK         1     // Упаковывать информацию при записи в базу

//-------- Тип информации в файле в формате обмена ИФ ---------------------
#define    DDC_TYPE_BASE     0       // База
#define          DDC_TYPE_STR      1       // Штрихи
#define          DDC_TYPE_DOC      2       // Документация
#define             DDC_TYPE_FILE     3       // Любые файлы
#define             DDC_TYPE_NONE     4       // Не определен

//------- Флаги открытия файла в формате обмена ----------------------------
#define   DDC_LIST_YES    1          // С формированием списка объектов для
                                                                                                                                                    //  быстрого доступа
#define   DDC_LIST_NO     0          // Без формирования списка объектов

//---------- Способ выделения памяти под части объекта ----------------------
#define  BASE_NEW_FORMAT       0    // Память под объект выделяется системой
                                                                                                                                                // при чтении объекта из базы данных
                                                                                                                                                // Связи и прерывания представляются в
                                                                                                                                                // оперативной памяти в виде линейного
                                                                                                                                                // двунаправленного списка

#define  BASE_OLD_FORMAT       1    // Массивы под поля объекта подаются
                                                                                                                                                // пользователем
                                                                                                                                                // Связи и прерывания располагаются в
                                                                                                                                                // поданном пользователем массиве

#define  BASE_OLD_FORMAT_DYN   2    // Только для связей и прерываний:
#define OBJ_SCALE_01       0x80000000
#define OBJ_SCALE_02       0x40000000
#define OBJ_SCALE_03       0x20000000
#define OBJ_SCALE_04       0x10000000
#define OBJ_SCALE_05       0x08000000
#define OBJ_SCALE_06       0x04000000
#define OBJ_SCALE_07       0x02000000
#define OBJ_SCALE_08       0x01000000
#define OBJ_SCALE_09       0x00800000
#define OBJ_SCALE_10       0x00400000
#define OBJ_SCALE_11       0x00200000
#define OBJ_SCALE_12       0x00100000
#define OBJ_SCALE_13       0x00080000
#define OBJ_SCALE_14       0x00040000
#define OBJ_SCALE_15       0x00020000
#define OBJ_SCALE_16       0x00010000
#define OBJ_SCALE_17       0x00008000
#define OBJ_SCALE_18       0x00004000
#define OBJ_SCALE_19       0x00002000
#define OBJ_SCALE_20       0x00001000
#define OBJ_SCALE_21       0x00000800
#define OBJ_SCALE_22       0x00000400
#define OBJ_SCALE_23       0x00000200
#define OBJ_SCALE_24       0x00000100
#define OBJ_SCALE_25       0x00000080
#define OBJ_SCALE_26       0x00000040
#define OBJ_SCALE_27       0x00000020
#define OBJ_SCALE_28       0x00000010
#define OBJ_SCALE_29       0x00000008
#define OBJ_SCALE_30       0x00000004
#define OBJ_SCALE_31       0x00000002
#define OBJ_SCALE_32       0x00000001
//------------------- Для анализа логической шкалы ---------------------
#define BaseScaleOneYes  0   // Включен хотя бы один указанный разряд
#define BaseScaleAllYes  1   // Включены все указанные разряды
#define BaseScaleOneNo   2   // Выключен хотя бы один указанный разряд
#define BaseScaleAllNo   3   // Выключены все указанные разряды

//------------------ Типы значения характеристики -----------------------
#define  XAR_TYPE_INT         1     // Целое короткое (2 байта)
#define  XAR_TYPE_DOUBLE      2     // Вещественное с двойной точностью (8 байт)
#define  XAR_TYPE_LONG        3     // Целое длинное (4 байта)
#define  XAR_TYPE_TEXT        4     // Текст (1 - 255 байт)
#define  XAR_TYPE_STRUCT      5     // Структурное число (1 - 129 позиций)
#define  XAR_TYPE_FLOAT       6     // Вещественное короткое (4 байта)
#define  XAR_TYPE_NO          7     // Без значения
#define  XAR_TYPE_COORD       8     // Географическая координата (8 байт)
#define  XAR_TYPE_DATE        9     // Дата (4 байта)
#define  XAR_TYPE_COMPLEX    10     // Сложная характеристика
#define  XAR_TYPE_RECORD     11     // Запись

//------------- Флаги для работы с характеристиками ------------------------
#define  OPEN_SEM         0    // Чтение характеристик потоком
#define  READ_SEM         1    // Чтение одной характеристики
#define  NEXT_SEM         2    // Чтение следующей характеристики
                                                                                                                            //  в открытом запросе на чтение
                                                                                                                            //  характеристик потоком
#define UPDATE_SEM        3    // Замена или добавление характеристики
#define CHANGE_SEM        4    // Только замена характеристики
#define DELETE_SEM        5    // Удаление характеристики
#define ADD_SEM_CODE      6    // Добавление характеристики перед
                                                                                                                            // характеристикой с указанным кодом
#define ADD_SEM_NUMBER    7    // Добавление характеристики перед
                                                                                                                            //  характеристикой с указанным
                                                                                                                            //  порядковым номером
#define DELETE_SEM_CP     8    // Удаление характеристики, указанной
                                                                                                                            //  кодом и точками привязки
#define DELETE_SEM_NUMBER 9    // Удаление характеристики по порядковому номеру
#define  OPEN_SEM_CODE   10    // Чтение  потоком характеристик,
                               // заданных полным кодом
#define BASE_FEAT_LOC_OBJ   1  // Читать характеристики из объекта
#define BASE_FEAT_LOC_SV    2  // Читать характеристики из связи
#define BASE_FEAT_LOC_PR    3  // Читать характеристики из прерывания
#define BASE_FEAT_LOC_FIELD 4  // Читать характеристики из произвольного поля

//-------------- Флаги для чтения и поиска объектов в базе ------------------
#define  OPEN_TREE           0     // Чтение потоком с обходом поддерева
#define  OPEN_CODE_SELECT    1     // Выборочное чтение про номеру
                                                                                                                                            //  объектов заданного полного кода
#define  OPEN_CODE           2     // Чтение потоком объектов
                                                                                                                                            //  заданного полного кода
#define  OPEN_LIST           0     // Чтение потоком по инвертированному списку
#define  READ_NEXT           3     // Чтение следующего объекта в потоке
#define  READ_RETRY          4     // Повторное чтение только что
                                                                                                                                            //  прочитанного в потоке объекта
#define  READ_WR             5     // Обновление только что прочитанного
                                                                                                                                            //  в потоке объекта
#define  READ_SELECT         6     // Выборочное чтение объекта по
                                                                                                                                            //  номеру
#define  READ_RESTORE        7     // Восстановление запроса
#define  READ_DELETE         8     // Удаление частей только что
                                                                                                                                            //  прочитанного а потоке объекта
#define  READ_SIZE           9     // Определение размеров только
                                                                                                                                            //  что прочитанного в потоке
                                                                                                                                            //  объекта
#define  READ_RIGHT         10     // Чтение вправо по уровню
#define  READ_LEFT          11     // Чтение влево по уровню
#define  READ_UP            12     // Чтение вверх по дереву
#define  READ_DOWN          13     // Чтение вниз по дереву
#define  READ_FIRST         14     // Чтение первого на уровне
#define  READ_LAST          15     // Чтение последнего на уровне
#define  READ_CLOSE         16     // Принудительное закрытие запроса
#define  BASE_FIND_OPEN      0     // Открытие запроса на координатный поиск объекта
#define  BASE_FIND_NEXT      1     // Поиск следующего объекта

#define  BASE_FIND_CLOSE     2     // Закрытие запроса

//----------- Покоординатные поисковые флаги ---------------------------------
#define  FIND_BY_METRICS     1     // Поиск по метрике
#define  FIND_BY_SEM         2     // Поиск по координатам точек привязки характеристик объекта
#define  FIND_BY_SV          4     // Поиск по координатам точек привязки характеристик связей
#define  FIND_BY_PR          8     // Поиск по координатам точек привязки характеристик прерываний
#define  FIND_BY_ALL        15     // Полный поиск
#define  FIND_DO            16     // Поиск по координатам точек привязки
                                   //  дискретного объекта
#define  FIND_PRECISION_SMALL  0   // Для поиска по точкам метрики
#define  FIND_PRECISION_LARGE  1   // Для поиска по отрезкам метрики

//--------------- Флаги выборки частей объекта ------------------------------
#define  OBJ_CODE            0     // Только идентификационная часть объекта:
                                                                                                                                            //  код, номер, ключ, шкала
#define  OBJ_SEM             1     // Семантика
#define  OBJ_SV              2     // Связи
#define  OBJ_PR              4     // Прерывания
#define  OBJ_SQUARE          8     // Квадраты
#define  OBJ_METRIC         16     // Метрика
#define  OBJ_ALL            31     // Весь объект без текста и поля мультимедиа)
#define  OBJ_TEXT           32     // Текст
#define  OBJ_VIDEO          64     // Поле мультимедиа
#define  OBJ_FULL          127     // Чтение всего объекта
#define  OBJ_SEM_HEX       128     // Печать х-ки в шестнадц.формате
#define  OBJ_VIDEO_ALL       2     // Дополнительный флаг в дескриптор объекта
                                                                                                                                            //  для чтения всего поля мультимедиа
#define  OBJ_SEM_PARTS     256     // Дополнительный режим сборки всех характеристик
                                   // распределенного объекта из его частей
#define  OBJ_SV_PARTS      512     // Дополнительный режим сборки всех связей
                                   // распределенного объекта из его частей
#define  OBJ_PR_PARTS     1024     // Дополнительный режим сборки всех прерываний
                                   // распределенного объекта из его частей
#define  OBJ_VIDEO_PARTS  2048     // Дополнительный режим сборки полей мультимедиа
#define  OBJ_PR_SORT      4096     // Дополнительный режим сортировки прерываний
                                   //  при чтении распределенного объекта
#define  OBJ_NUMBER       8192
#define  OBJ_SCALE       16384
#define  OBJ_CODE_SVPR   32768
#define  OBJ_VIDEO_EXT   65536
#define  OBJ_VIDEO_NAME 131072

//------------------ Флаги записи объекта -----------------------------------
#define  WRITE_DELETE        0     // Запись объекта с удалением старого
#define  WRITE_ADD           1     // Запись объекта логическим
                                                                                                                                            //  сложением
#define  WRITE_NO            2     // Не записывать объект если он
                                                                                                                                            //  уже есть
#define  WRITE_AUTO          3     // Запись объекта с автоматической
                                                                                                                                            // нумерацией
//---------- Дополнительные флаги только при записи объекта в режиме WRITE_ADD
#define  WRITE_SV_DELETE     0     // Удаление старых связей и
                                                                                                                                            //  запись новых
#define  WRITE_SV_ADD        4     // Добавление новых связей
                                                                                                                                            //  логическим сложением
#define  WRITE_SV_NO         8     // Не записывать связь если она уже есть
#define  WRITE_SV_AUTO      12     // Добавление новых связей к старым
#define  WRITE_PR_DELETE     0     // Аналогично связям для прерываний
#define  WRITE_PR_ADD       16     //
#define  WRITE_PR_NO        32     //
#define  WRITE_PR_AUTO      48     //

//------------------ Флаги для полей связей и прерываний -----------------
#define  BASE_SV             0   // Связи
#define  BASE_PR             1   // Прерывание
#define  BASE_SVPR_NORENUM   2   // Не перенумеровывать ключи связей/прерываний
#define  BASE_SVPR_RENUM     0   // Перенумеровывать ключи связей/прерываний
#define  BASE_SVPR_FIRST    -1   // Вставка связи в начало списка
#define  BASE_SVPR_LAST     -2   // Вставка связи в конец списка
#define  BASE_SVPR_ADR      -3   // Вставка по адресу
#define  BASE_SVPR_POINT    -4   // Вставка по номеру точки в метрике
#define  BASE_SVPR_OPEN      0   // Открыть запрос на чтение связей/прерываний
#define  BASE_SVPR_READ      1   // Прочитать очередную связь
#define  BASE_SVPR_READOR    2   // При отборе - условие "ИЛИ"
#define  BASE_SVPR_READAND   0   // ПРи отборе - условие "И"
#define  SVPR_OPEN           0
#define  SVPR_READ           2

//-------------- Флаги для работы со списком окон --------------------------
#define  LISTKV_ALL          0   // Чтение всего описания списка окон
#define  LISTKV_HEADER       1   // Чтение только заголовка
                                                                                                                                    //  информации о списке окон
#define  LISTKV_UPDATE       2   // Замена существующего описания
                                                                                                                                    //  списка окон
#define  LISTKV_NOUPDATE     3   // Запрет замены существующего
                                                                                                                                    //  описания списка окон
#define  BASE_BEGIN_LISTKV   30000   // Начало списка окон в инвертированных
                                                                                                                                                    // списках. Окно 1 имеет номер списка
                                                                                                                                                    // 30001

//----------------- Флаги для работы с системным журналом -----------
#define  SYSJN_ON            0   // Подключение журнала к базе
#define  SYSJN_OFF           1   // Отключение журнала
#define  SYSJN_INFO          2   // Получение информации от наличии
                                                                                                                                    //  журнала у базы

//---------------- Флаги для работы с классификатором -----------------------
#define  CLASS_OBJECT        1   // Чтение информации об объекте
#define  CLASS_FEATURE                            2   // Чтение информации о характеристике

//----------------- Тип координаты метрического описания ------------------
#define  BASE_MET_COUNT_2    0   // Размерность 2
#define  BASE_MET_COUNT_3    8   // Размерность 3
#define  BASE_MET_INTEGER    0   // Целый
#define  BASE_MET_FLOAT      1   // Вещественный
#define  BASE_MET_LOGICAL    2   // Логический

//==================== Идентификатор характеристики ===============
typedef struct
{
    BASE_INT Code[10];    // Код характеристики (полный, обобщенный код или 0)
    unsigned long Number; // Порядковый номер характеристики данного
                       // полного или обобщенного кода (0 равносилен 1).
} FEATURE_CODE;
//==================== Значение характеристики ===================
typedef union
{
    BASE_INT I;               // Тип 1      Целый короткий
    double D;                 // Тип 2      Вещественный двойной точности
    long L;                   // Тип 3      Целый длинный
    char T[260];              // Тип 4      Текст. Тексты в базе данных всегда
                                                                                                            //            представлены в кодировке MS DOS
                                                                                                            //            При выдаче пользователю они автоматически
                                                                                                            //            перекодируются в кодировку ANSI Windows
    BASE_INT S[130];          // Тип 5      Структурное число
    float  F;                 // Тип 6      Вещественный короткий
    struct                    // Тип 8      Географическая координата:
    {
        BASE_INT gm[2];          //            Градусы и минуты
        float c;                 //            Секунды
    } gmc;
    unsigned long DATE;       // Тип 9      Дата (абсолютный номер дня от
                                                                                                            //                    01.01.1900)
    BASE_INT *pComplex;       // Тип 10     Сложная характеристика. Указатель на
                           //            значение сложной х-ки
    BASE_INT *pRecord;        // Тип 11     Запись. Указатель на массив.
                           //            1-е слово - размер записи в байтах
                           //            без учета этого слова
} BASE_XAR_VALUE;
//================ Элемент заголовка поля мультимедиа ======================
typedef struct
{
    unsigned long Number;   // Номер элемента мультимедиа (> 0)
 char Description[256];  // Описание элемента
 char Type[8];           // Тип элемента (расширение файла, например, BMP)
 int Location;           // Местоположение элемента:
                         //  MULTI_BASE - в объекте базы данных
                         //  MULTI_FILE - в отдельном файле
 char FileName[256];     // Имя файла (с расширением) с элементом при Location=MULTI_FILE
 int Action;             // Флаг обработки элемента при записи объекта в базу:
                         //  MULTI_NO - не записывать в базу
                         //  MULTI_DEL - удалить в базе
                         //  MULTI_ADD - добавить в поле мультимедиа объекта в базе
 unsigned long Size;     // Размер элемента в байтах
    unsigned long Adr;      // Адрес начала элемента в поле мультимедиа
                                                                                                    //  при выдачи этого поля целиком
} VIDEO_HEADER;
//========= Структура описателя списка окон (квадратов) базы данных ====
typedef struct
{
    double Xmin;    // Координаты левого нижнего и правого верхнего
    double Ymin;    //  угла прямоугольной рамки, охватывающей базу
    double Xmax;    //  (в единицах базы данных)
    double Ymax;
    long Kx;        // Количество окон по горизонтали
    long Ky;        // Количество окон по вертикали
    double Dx;      // Размер окна по горизонтали (в единицах базы)
    double Dy;      // Размер окна по вертикали (в единицах базы)
    long Count;     // Количество окон
} BASE_LISTKV;
//========= Для выдачи информации о найденном по метрике объекте =====
typedef struct
{
 char FileName[256];    // Полное имя файла базы данных
 BASE_INT Code[10];     // Код объекта
 unsigned long Number;  // Номер объекта
 BASE_INT64 Scale;      // Логическая шкала объекта
 double X;              // Координаты ближайшей точки в найденном объекте
 double Y;
 int Type;              // Тип информации, по которой найден объект:
                        //  FIND_BY_METRICS - по метрике объекта
                        //  FIND_BY_SEM - по характеристикам обекта
                        //  FIND_BY_SV - по характеристикам связи
                        //  FIND_BY_PR - по характеристикам прерывания
 int NumberPoint;       // Порядковый номер ближайшей точки в метрике объекта или
                        //  характеристики найденного объекта
 int NumberSv;          // Порядковый номер связи, по характеристике
                        //  которой найден объект
 int NumberPr;          // Порядковый номер прерывания, по характеристике
                        //  которого найден объект
 BASE_INT64  ScaleSvPr; // Логическая шкала прерывания или связи
 BASE_INT CodeSvPr[10]; // Код прерывающего/связанного объекта, если ведущий объект
                        //  найден по характеристике прерывания/связи
 BASE_INT CodeSem[10];  // Код характеристики, по точкам которой найден объект
 int NumberSem[102];    // Порядковые номера характеристики, по которой
                        //  найден объект (по уровням вложенности в сложные
                        //  характеристики). NumberSem[0] - количество уровней
 float FindTime;        // Время поиска в секундах
 double Distance;       // Расстояние до ближайшей точки в мм
} GBASE_FIND_ANY_OBJECT;
//=== Элемент списка объектов, которые не участвуют в поиске по метрике ======
typedef struct
{
 BASE_INT Code[10];     // Код объекта
 unsigned long Number;  // Номер объекта (если 0, то Code указывает
                        //  на группировку объектов)
 int TypeFind;          // Тип информации, по которой объект не участвует
                        //  в поиске (суперпозиция):
                        //  FIND_BY_METRICS
                        //  FIND_BY_SEM
                        //  FIND_BY_SV
                        //  FIND_BY_PR
                        //  FIND_BY_ALL или 0 - объект совсем не участвует в поиске
} GBASE_NOT_FIND_OBJECT;
//================= Структура дескриптора объекта  =====================
//    Дескриптор (описатель) объекта заполняется при чтении объекта из
//    базы данных. Перед чтением объекта его описатель должен быть
//    инициализирован функцией BaseInitObject.
//    Перед записью объекта в базу пользователь должен сформировать в
//    описателе объекта все необходимые для записи поля.
//    По окончании работы с объектом он (а соответственно и его описатель)
//    должен быть закрыт функцией BaseCloseObject
//
typedef  struct
{
    unsigned long   Number;        // Номер объекта (выдается при чтении,
                                                                                                                                //  подается при записи)
    unsigned long   NumberNew;     // Заполняется системой после записи объекта
                                                                                                                                //  в базу: номер объекта, под которым
                                                                                                                                //  он записан в базу
    int       Scale;        // Логическая шкала объекта (выдается при чтении,
                                                                                                                                //  подается при записи)
    unsigned int  FlagScale;       // Флаг записи шкалы объекта. Должен быть
                                                                                                                                // сформирован перед записью объекта в базу:
                                                                                                                                //     1 - записывать шкалу объекта
                                                                                                                                //     0 - не записывать шкалу
    long           Key;            // Уровень (ключ) доступа к объекту (выдается
                                                                                                                                //  при чтении, подается при записи)
    unsigned int   FlagKey;        // Флаг записи ключа объекта. Должен быть
                                                                                                                                // сформирован перед записью объекта в базу:
                                                                                                                                //     1 - записывать ключ объекта
                                                                                                                                //     0 - не записывать ключ

//---------- Указатели на части объекта или NULL при отсутствии
//  Заполняются системой при динамическом способе выделения памяти
//  или формируются пользователем, в том случае когда он сам выделяет память
//  под соответствующие части объекта
    BASE_INT       *pSem;          // СЕМАНТИКА ( Размер до 65536 байт)
    void           *pSv;           // СВЯЗИ. Имеется три формы представления
                                                                                                                                // этого поля:
                                                                                                                                //  ФОРМА 1 (новый формат). Связи организованы
                                                                                                                                // в виде двунаправленного симметричного
                                                                                                                                // списка. Каждая связь имеет
                                                                                                                                // структуру типа BASE_SVPR.
                                                                                                                                // Всего может быть до 2**31 связей
                                                                                                                                //  ФОРМА 2 (старый формат). Связи лежат в
                                                                                                                                // одном линейном массиве. Каждая
                                                                                                                                // связь имеет структуру BASE_SVPR_OLD
                                                                                                                                // Массив подается пользователем
                                                                                                                                //  ФОРМА 3 (старый-динамический формат).
                                                                                                                                // Связи лежат в одном линейном массиве.
                                                                                                                                // Каждая связь имеет структуру BASE_SVPR_OLD
                                                                                                                                // Память под массив берется динамически
    void           *pPr;           // ПРЕРЫВАНИЯ. Имеется три формы представления:
                                                                                                                                //  ФОРМА 1 (новый формат). Прерывания
                                                                                                                                // организованы в виде двунапрвленного
                                                                                                                                // симметричного списка.
                                                                                                                                // Каждое прерывание имеет
                                                                                                                                // структуру типа BASE_SVPR.
                                                                                                                                // Всего может быть до 2**31 прерываний
                                                                                                                                //  ФОРМА 2 (старый формат). Прерывания
                                                                                                                                // лежат в одном линейном массиве.
                                                                                                                                // Каждое прерывание имеет структуру
                                                                                                                                // BASE_SVPR_OLD
                                                                                                                                // Массив подается пользователем
                                                                                                                                //  ФОРМА 3 (старый-динамический формат).
                                                                                                                                // Прерывания лежат в одном линейном массиве.
                                                                                                                                // Каждое прерывание имеет структуру BASE_SVPR_OLD
                                                                                                                                // Память под массив берется динамически
    void       *pSquare;           // КВАДРАТЫ. Количество квадратов (4-х байтовых
                                                                                                                                //  элементов) вынесено в отдельное поле
                                                                                                                                //  qSquare. Размер поля квадратов
                                                                                                                                //  не ограничен. Поле квадратов имеет
                                                                                                                                //  произвольную структуру
    void       *pMet;              // МЕТРИКА. Количество точек в метрике
                                                                                                                                //  вынесено в отдельное поле qMet.
                                                                                                                                //  Размер поля метрики не ограничен.
                                                                                                                                //  Описание элемента информации в поле
                                                                                                                                //  метрики дается переменными
                                                                                                                                //  CountItemsMet, SizeItemMet и TypeItemMet
                                                                                                                                //  в блоке управления открытой базой данных
    char       *pText;             // ТЕКСТ. Текстовое описание объекта (размер
                                                                                                                                //  в байтах в поле qText и sText -
                                                                                                                                //  до 65536 байт)
    VIDEO_HEADER *pVideo;          // МУЛЬТИМЕДИА. Поле мультимедиа объекта
                                                                                                                                //  может быть представлено в двух формах:
                                                                                                                                // ФОРМА 1. Дескриптор поля мультимедиа
                                                                                                                                //  объекта в структуре VIDEO_HEADER
                                                                                                                                // ФОРМА 2. Дескриптор поля мультимедиа
                                                                                                                                //  объекта, а за ним все элементы поля
                                                                                                                                //  мультимедиа в порядке их представления
                                                                                                                                //  в дескрипторе
//************* Правила записи в базу поля мультимедиа функцией BaseWriteObject.
//*** Для ФОРМЫ 2: все поданное поле мультимедиа записывается в базу (или полностью
//                 заменяет его в существующем объекте в режиме WRITE_ADD)
//*** Для ФОРМЫ 1: в базу данных добавляются только элементы мультимедиа,
//                 помеченные в дескрипторе Action=MULTI_ADD. Если Location=MULTI_FILE,
//                 добавляется ссылка на файл. Если Location=MULTI_BASE. то элемент
//                 из указанного файла переписывается в объект базы данных.
//                 Если объект уже существовал и в нем было поле мультимедиа,
//                 то элементы, помеченные ACTION=MULTI_DEL, будут удалены из базы.
//                 Если Action=MULTI_DSC, то у существующего в объекте базы элемента
//                 изменяется описание на поданное в Description.
//                 Элементы, помеченные Action=MULTI_NO, не принимают участие
//                 в обработке.
//                 Для управления элементами в дескрипторе имеются функции:
//         BaseMultiMediaItemAdd, BaseMultiMediaItemDel, BaseMultiMediaItemNo,
//         BaseGetMultiMediaItem

//--------- Флаги способа выделения памяти под части -----------------
//          Должны устанавливаться пользователем перед работой с объектом
//          Для этого есть функция BaseInitObject
    unsigned int   FlagSem;        // Формат представления поля характеристик:
                                                        //  BASE_OLD_FORMAT - чтение характеристик
                                                        //    производится в заранее подан-
                                                        //    ное в pSem поле размером mSem байт
                                                        //  BASE_NEW_FORMAT - память под поле х-к
                                                        //    берется динамически независимо от пользователя
    unsigned int   FlagSv;         // Формат представления поля связей:
                                                        //  BASE_OLD_FORMAT - ФОРМА 2. Связи читаются в поданный
                                                        //      пользователем массив
                                                        //  BASE_NEW_FORMAT - ФОРМА 1: связи выдаются в виде списка
                                                        //                (память выделяется динамически)
                                                        //  BASE_OLD_FORMAT_DYN - ФОРМА 3. Память берется независимо
                                                        //       от пользователя. Связи выдаются в одном массиве
    unsigned int   FlagPr;         // Формат представления поля прерываний:
                                                        //  BASE_OLD_FORMAT - ФОРМА 2. Прерывания читаются в поданный
                                                        //    пользователем  массив
                                                        //  BASE_NEW_FORMAT - ФОРМА 1: прерывания выдаются в виде списка
                                                        //                (память выделяется динамически)
                                                        //  BASE_OLD_FORMAT_DYN - ФОРМА 3. Память берется независимо
                                                        //       от пользователя. Прерывания выдаются в одном массиве
unsigned int  FlagSquare;     // Формат представления поля квадратов:
                                                        //  BASE_OLD_FORMAT - квадраты читаются в поданное
                                                        //   пользователем поле размером mSquare байт
                                                        //  BASE_NEW_FORMAT - память под поле квадратов
                                                        //   берется независимо от пользователя
    unsigned int   FlagMet;        // Формат представления поля метрики:
                                                        //  BASE_OLD_FORMAT - метрика читается в поданное
                                                        //   пользователем поле размером mMet байт
                                                        //  BASE_NEW_FORMAT - память под поле метрики
                                                        //   берется независимо от пользователя
    unsigned int   FlagText;       // Формат представления поля текста:
                                                        //  BASE_OLD_FORMAT - текст читается в поданное
                                                        //   пользователем поле размером mText байт
                                                        //  BASE_NEW_FORMAT - память под поле текста
                                                        //   берется независимо от пользователя
    unsigned int   FlagVideo;      // Формат представления поля мультимедиа:
                                                        //  ...1 - дескриптор или все поле читается в
                                                        //    поданное поле размером mVideo байт (BASE_OLD_FORMAT)
                                                        //  ...0 - память под поле мультимедиа
                                                        //   берется независимо от пользователя (BASE_NEW_FORMAT)
                                                        //  ..1. - поле содержит всю информацию вместе с дескриптором
                                                        //         (BASE_NEW_FORMAT | OBJ_VIDEO_ALL
              //          или BASE_OLF_FORMAT | OBJ_VIDEO_ALL)
//--------- Размеры полей под части объекта  -----------------
//          Должны быть заданы перед чтением только для
//          режима BASE_OLD_FORMAT
//          При записи объекта не используются
    long           mSem;           // Размер в байтах поля pSem при
                                                                                                                                //  FlagSem=BASE_OLD_FORMAT
    long           mSv;            // Размер в байтах массива Sv для ФОРМЫ 2
    long           mPr;            // Размер в байтах массива Pr для ФОРМЫ 2
    long           mSquare;        // Размер в байтах поля pSquare
                                                                                                                                // при FlagSquare=BASE_OLD_FORMAT
    long           mMet;           // Размер в байтах поля pMet
                                                                                                                                // при FlagMet=BASE_OLD_FORMAT
    long           mText;          // Размер в байтах поля pText
                                                                                                                                // при FlagText=BASE_OLD_FORMAT
    long           mVideo;         // Размер в байтах поля pVideo
                                                                                                                                // при FlagVideo=...1
//----------------- Количественные сведения ------------------------
//   Выдаются при чтении
//   Должны быть заданы при записи
    long           qSem;           // Количество характеристик
                                                                                                                                //  (пока всегда 0)
    long           qSv;            // Количество связей
    long           qPr;            // Количество прерываний
    long           qSquare;           // Количество квадратов
                                                                                                                                //  (элементов по 4 байта)
    long           qMet;           // Количество точек (элементов) в метрике
    long           qText;          // При выдаче всегда равно sText
    long           qVideo;         // Количество элементов в поле мультимедиа
//---------- Размеры в байтах частей объекта (формируются при чтении) --------
//           При записи не используются
    long           sSem;           // Семантика (с учетом первого слова)
    long           sSv;            // Поле связей
    long           sPr;            // Поле прерываний
    long           sSquare;        // Поле квадратов. Всегда кратно 4-м байтам
    long           sMet;           // Поле метрики
    long           sText;          // Размер в байтах текстового описания
    long           sVideo;         // Размер в байтах поля мультимедиа
//-------------------------------------------------------------------------
    int            Error[3];       // Расширенный код ошибки
                                                                                                                                // (см.функцию BaseCheckObject)
//------------------------------------------------------------------------
    BASE_INT      Code[10];        // Код объекта
    unsigned int  FlagReadWrite;   // Флаг чтения/записи объекта:
                                                    //  0 - объекта нет
                                                    //  1 - объект прочитан. Устанавливается
                                                    //      после чтения объекта из базы
                                                    //  2 - объект записан в базу.
                                                    // Устанавливается после записи объекта в базу
                                                    // Сбрасывается в 0 при выполнении функций
                                                    //  BaseInitObject, BaseCloseObject
} GBASE_OBJECT;

//= Структура постоянной части связи/прерывания (для формата BASE_NEW_FORMAT) ===
typedef struct
{
    BASE_INT      *pBack;    // Указатель на предыдущую связь (у первой NULL)
    BASE_INT      *pForward; // Указатель на следующую связь (у последней NULL)
    unsigned long  Size;     // Размер связи/прерывания в словах по 2 байта
    unsigned long  Key;      // Ключ связи (0 - 2**31)
                                                                                                        // (если 0, то в записи в базу данная
                                                                                                        // связь/прерывание не участвует)
    BASE_INT      Code[10];  // Код связанного/прерывающего объекта
    unsigned long  Number;   // Номер связанного/прерывающего объекта
    unsigned long  Point;    // Номер точки в метрике для прерывания
    int            Scale;  // Логическая шкала
    BASE_INT      Sem;       // Размер поля характеристик в словах по 2 байта
                                                                                                        // (или 0, если характеристик нет)
                                                                                                        //  Адрес (BASE_INT *)&Sem - начало поля семантики
                                                                                                        //  связи/прерывания
} BASE_SVPR;

//======= Структура постоянной части связи/прерывания для форматов ==========
//        BASE_OLD_FORMAT или BASE_OLD_FORMAT_DYN
typedef struct
{
    unsigned long Size;      // Размер связи/прерывания в словах по 2 байта
    unsigned long Key;       // Ключ связи (0 - 2**31)
                                                                                                        // (если 0, то в записи не участвует)
    BASE_INT      Code[10];  // Код связанного объекта
    unsigned long  Number;   // Номер связанного объекта
    unsigned long  Point;    // Номер точки в метрике для прерывания
    int Scale;        // Логическая шкала
    BASE_INT      Sem;       // Размер поля характеристик в словах по 2 байта
                                                                                                        // (или 0, если характеристик нет)
                                                                                                        //  Адрес (BASE_INT *)&Sem - начало поля семантики
                                                                                                        //  связи/прерывания
} BASE_SVPR_OLD;

//======= Структура служебной информации прочитанной характеристики =====
typedef struct
{
// .... выдаются при чтении характеристики;
// .... используются при добавлении или изменении характеристики
    BASE_INT Code[10];           // Код характеристики
    int      Type;               // Тип характеристики
    unsigned int  FlagValue;     // Флаг значения функции BaseReadFeature
                              // (возможные значения: VALUE_SEM0 - VALUE_SEM3)
    int qPoints;                 // Количество точек привязки
    void *AddrPoints;            // Указатель на начало точек привязки х-ки

// .... выдаются при чтении характеристики;
    BASE_INT *AddrF;             // Указатель на начало характеристики
    void *AddrValue;             // Указатель на начало значения х-ки
    unsigned long Numb;          // Порядковый номер характеристики во всем поле семантики
    unsigned long NumberCode;    // Порядковый номер характеристики среди характеристик,
                              //  прочитанных по указанному запросу
    int SizeValue;               // Длина значения характеристики в байтах

// ....используются только при добавлении или изменении характеристики
 int CountItemsMet;           // Количество составляющих точки привязки (1-255);
 int SizeItemMet;             // Размер составляющей точки привязки (1-255);
} BASE_FEATURE;
//==========================================================================
typedef struct
{
 BASE_INT Code[10];    // Код группы читаемых объектов/объекта
 int FlagCode;         // Тип запроса на чтение по коду объекта:
                       //  OPEN_TREE - по обобщенному коду
                       //  OPEN_CODE - только объекты заданного полного кода
 unsigned long Number; // Номер объекта (если читается только один объект. в этом
                       //  случае Code указывает полный код объекта) или 0, если
                       //  читается группа объектов
 char TextMKO[1000];   // Текст фильтра на языке или пустая строка
 COLORREF ColorContur; // Цвет изображения контура объекта
 COLORREF ColorSquare; // Цвет заливки площадного объекта
 int TypePen;          // Тип линии контура
 int WidthPen;         // Толщина линии контура
 int TypeBrush;        // Тип заливки площадного объекта
 int NumberLayer;      // Номер пластика (слоя)
 int TypeXorCopyLine;  // Наложение на другую картинку линий
 int TypeXorCopySq;    // Наложение на другую картинку заливок
} GBASE_MULTI_READ;

//=============== Структура сложного фильтра на отбор объектов ==========
typedef struct
{
 char Label[4];                   // Служебная метка сформированного фильтра
 char MultiFilterName[256];       // Идентификатор сложного фильтра (до 250 символов)
 char MultiFilterDescription[256];// Описание сложного фильтра (до 256 символов)
 int CountQuerys;                 // Количество фильтров в сложном фильтре (или 0)
 GBASE_MULTI_READ *Querys;        // Указатель на массив фильтров (или NULL)
} GBASE_MULTI_MKO;

//=============== Вспомогательные типы для метрического описания =============
// CountItemsMet=2 SizeItemMet=2 TypeItemMet=BASE_MET_INTEGER
typedef struct
{
    BASE_INT x;        // Координата X
    BASE_INT y;        // Координата Y
} MET_INT_2;

// CountItemsMet=3 SizeItemMet=2 TypeItemMet=BASE_MET_INTEGER
typedef struct
{
    BASE_INT x;        // Координата X
    BASE_INT y;        // Координата Y
    BASE_INT h;        // Высота
} MET_INT_3;

// CountItemsMet=2 SizeItemMet=4 TypeItemMet=BASE_MET_INTEGER
typedef struct
{
    int x;            // Координата X
    int y;            // Координата Y
} MET_LONG_2;

// CountItemsMet=3 SizeItemMet=4 TypeItemMet=BASE_MET_INTEGER
typedef struct
{
    long x;            // Координата X
    long y;            // Координата Y
    long h;            // Высота
} MET_LONG_3;

// CountItemsMet=2 SizeItemMet=4 TypeItemMet=BASE_MET_FLOAT
typedef struct
{
    float x;           // Координата X
    float y;           // Координата Y
} MET_FLOAT_2;

// CountItemsMet=3 SizeItemMet=4 TypeItemMet=BASE_MET_FLOAT
typedef struct
{
    float x;           // Координата X
    float y;           // Координата Y
    float h;           // Высота
} MET_FLOAT_3;

// CountItemsMet=2 SizeItemMet=8 TypeItemMet=BASE_MET_FLOAT
typedef struct
{
    double x;          // Координата X
    double y;          // Координата Y
} MET_DOUBLE_2;

// CountItemsMet=3 SizeItemMet=8 TypeItemMet=BASE_MET_FLOAT
typedef struct
{
    double x;           // Координата X
    double y;           // Координата Y
    double h;           // Высота
} MET_DOUBLE_3;

//----------- Список кодов
typedef struct
{
 BASE_INT Code[10];      // Код объекта или группы объектов
 int TypeCode;           // Тип указанного кода:
                         //  OPEN_TREE - дерево объектов
                         //  OPEN_CODE - только объекты указанного кода
 unsigned long Number;   // Номер объекта. Только для случая одного
                         //  объекта полного кода Code (TypeCode=OPEN_CODE)
} BASE_LIST_CODE;
//===== Блок управления открытой базой данных в формате обработки ==========
typedef struct
{
    char Label[8];            // Метка блока управления базой данных
    char OpenReg[4];          // Режим открытия базы
    TREE *base;               // Указатель на простое дерево СУБД НИКА
    char PassWord[44];        // Пароль
    int  FlagOpen;            // Флаг открытия базы:
                                                                                                            //  1 - Чтение (R)
                                                                                                            //  2 - Обновление (U)
                                                                                                            //  4 - Создание (C)
    int BaseFormat;           // Формат базы данных:
                                                                                                            //  BASE_FORMAT_2 или BASE_FORMAT_3
    int CountItemsMet;        // Количество составляющих элемента метрики
                                                                                                            //  (размерность) 1 - 255
    int SizeItemMet;          // Размер в байтах составляющей элемента метрики
                                                                                                            //   1 - 255
    int TypeItemMet;          // Тип составляющей элемента метрики:
                                                                                                            //  BASE_MET_INTEGER - целый
                                                                                                            //  BASE_MET_FLOAT   - вещественный
                                                                                                            //  BASE_MET_LOGICAL - логический
    float UnitMet;            // Единицы измерения метрики (количество в
                                                                                                            //  одном миллиметре)
    unsigned long Read;       // Количество операций чтения за сеанс
    unsigned long Write;      // Количество операций записи за сеанс
    unsigned long Delete;     // Количество операций удаления за сеанс
    char FileName[256];   // Полное имя файла базы данных
} GBASE;
//================ СТРУКТУРА ЗАПРОСА НА ЧТЕНИЕ =================
typedef  struct
{
//------ Пользовательская часть, которую он должен заполнять ---
//       с помощью функции BaseInitQuery
                        long          Key;               // Уровень доступа к объекту
                        int           Ident;             // Номер запроса (любое число)
//---------- Служебная часть ------------------------------------
//  Не для пользователей !!!
                        int                 Level;       // Уровень корня поддерева
                                                                                                                                                            //  запроса
                        int                 Current;     // Уровень вершины "o" теку-
                                                                                                                                                            //  щего объекта
                        unsigned long Num;               // Номер текущего объекта                                       //  щего объекта запроса
                        BASE_INT           Code[10];     // Код текущего объекта
                        int      Type;                   // Тип открытия запроса:
                                                                                                                                                            //  OPEN_CODE - чтение потоком
                                                                                                                                                            //    с обходом поддерева
                                                                                                                                                            //  OPEN_CODE_SELECT - вы-
                                                                                                                                                            //    борочное чтение объек-
                                                                                                                                                            //    тов заданного кода
                                                                                                                                                            //  OPEN_CODE - чтение потоком
                                                                                                                                                            //    объектов только задан-
                                                                                                                                                            //    ного полного кода
        int                FormatFlag;       // 0 - Old Format
                                                                                                                                                            // 1 - New Format
        int                r1;               // Резерв
        int                r2;               // Резерв
        int            FlagKV;               // Флаг для чтения по квадратам
        int            NumberKV;             // Текущий номер квадрата
        GBASE *BaseKV;                       // Указатель на рабочую базу
                                                                                                                                                            // со списком объектов при
                                                                                                                                                            // чтении по списку квадратов
        char *NameKV;                        // Указатель на имя временного
                                                                                                                                                            // файла рабочей базы
} GBASE_QUERY;
//=============== СТРУКТУРА СПИСКА КОДОВ И НОМЕРОВ ОБЪЕКТОВ =========
typedef struct
{
 BASE_INT Code[10];
 unsigned long Number;
} GBASE_LIST_OBJECTS;
//=============== СТРУКТУРА ЗАПРОСА НА СЛОЖНОЕ ЧТЕНИЕ ================
typedef struct
{
//------------- Для пользователей
 int Key;                  // Уровень доступа к объектам
 int Number;               // Номер запроса
 int Status;               // Статус запроса:
                           //  0 - закрыт
                           //  1 - инициализирован
                           //  2 - открыт
//------------- Служебная часть
 GBASE *Base;              // указатель на базу данных
 int CountQuerys;          // количество фильтров
 GBASE_MULTI_READ *Querys; // Указатель на массив фильтров
 BASE_INT MKO_OBJ[500];    // Текущий оттранслированный фильтр на отбор объектов
 BASE_INT *MKO;
 int CurrentQuery;         // номер текущего фильтра
 GBASE_QUERY Query;        // текущий запрос
 int FlagCode;             // Тип кода: OPEN_TREE или OPEN_CODE
 int FlagQuery;            // Флаг текущего запроса:
                           //  1 - открыт
                           //  0 - нет
//---- для чтения по инвертированным спискам
 int MaxCountObjects;      // Максимальный размер списка объектов
 int CurrentList;          // Индекс номера текущего списка
 int *AdrList;             // Массив номеров списков
 int CountReadObjects;            // Количество уже прочитанных объектов
 GBASE_LIST_OBJECTS *ListObjects; // Список кодов и номеров прочитанных объектов
} GBASE_MULTI_QUERY;
//============= СТРУКТУРА ЗАПРОСА НА ЧТЕНИЕ СВЯЗЕЙ/ПРЕРЫВАНИЙ ===========
//  Не для пользователей !!!
typedef struct
{
    unsigned long Q;            // Оставшееся количество связей
    BASE_SVPR *Adr;        // Адрес текущей связи
    unsigned long Num;          // Порядковый номер текущей связи
    unsigned long NS;           // Адрес связи в МКО
} BASE_ZAP_SP;

//================ СТРУКТУРА ЗАПРОСА НА ЧТЕНИЕ ХАРАКТЕРИСТИК =============
//  Не для пользователей !!!
typedef struct
{
    BASE_INT a[26];
    BASE_INT *pComplex;   // Адрес начала сложной характеристики
    void *AdrFeature;     // Указатель на поле характеристик
    unsigned long Numb;       // Порядковый номер х-ки, прочитанной в запросе
    unsigned long NumberCode; // Порядковый номер характеристики среди характеристик,
                                                                                                            //  прочитанных по указанному запросу
    int Pr_otk3;              // Признак открытия запроса; 1-открыт;0-нет
    FEATURE_CODE *CodeXar;    // Адрес идентификатора х-ки нижнего уровня
} SEM_QUERY;




HINSTANCE Base01LoadFun1(
 int *err,         // Код завершения:
                   // BERR_OK - адреса всех функций загружены
                   // BERR_NOINF - хотя бы одна функция не найдена
 GBASE_FUN01 *Fun  // Битовый массив флагов функций. Если бит = 1,
                   //  функция найдена, если = 0, то не найдена
                   // Если Fun==NULL, то битовый массив не формируется
);

// Функция возвращает Handle библиотеки или NULL в случае ее отсутствия

typedef int (BASEEXPORTFUNC *BaseCodeStr)(short int *Code,char* s);
typedef GBASE *(BASEEXPORTFUNC *BaseOpen)(int *RetCode,char* FileName, char *Function,
                                      char *Password,char *BaseName);
typedef int (BASEEXPORTFUNC *BaseClose)(GBASE *base);


typedef char * (BASEEXPORTFUNC *BaseOpenTextError)(int err,char *r);
typedef int (BASEEXPORTFUNC *BaseSetFormat)(int BaseFormat);

typedef int (BASEEXPORTFUNC *BaseSetMetDescriptor)(GBASE *Base,int Flag,int CountItemsMet,
    int SizeItemMet,int TypeItemMet,float UnitMet);
typedef int (BASEEXPORTFUNC *BaseClose)(GBASE *base);
typedef int (BASEEXPORTFUNC *BasePackMode)(GBASE *base,int Mode);
typedef int (BASEEXPORTFUNC *BaseReadObject)(GBASE *base,GBASE_QUERY *Query,
    BASE_INT *Select,int Fun,int Read,GBASE_OBJECT *Object,BASE_INT *Code,unsigned long Number);
typedef int (BASEEXPORTFUNC *BaseReadNumbers)(GBASE *base,BASE_INT *CodeObj,
    unsigned long *num_min,unsigned long *num_max);
typedef int (BASEEXPORTFUNC *BaseSetupQuery)(GBASE *base,GBASE_QUERY *Query,
    BASE_INT *Select,int reg,BASE_INT *code_zap,BASE_INT *code,unsigned long numer);
typedef void (BASEEXPORTFUNC *BaseCloseObject)(GBASE_OBJECT *Object);
typedef int (BASEEXPORTFUNC *BaseReadNameClass)(GBASE *base,char *name);
typedef int (BASEEXPORTFUNC *BaseReadName)(GBASE *base,char *name);
typedef int (BASEEXPORTFUNC *BaseReadListkv)(GBASE *base,BASE_LISTKV *List);
typedef int (BASEEXPORTFUNC *BaseReadPassword)(GBASE *base,char *PassWord,
    int MaxCount,int *n);
typedef int (BASEEXPORTFUNC *BaseReadClassInfo)(GBASE *Class,int Fun,
    BASE_INT *Code,char *Name,BASE_INT *Descriptor);
typedef void (BASEEXPORTFUNC *BaseInitQuery)(GBASE_QUERY *Query,
    long Key,int Ident);
typedef int (BASEEXPORTFUNC *BaseInitObject)(GBASE_OBJECT *Object,
    BASE_INT *sem,long mSem,void *Sv,long mSv,void *Pr,long mPr,
    void *Sq,long mSq,void *Met,long mMet,char *Text,long mText,
    VIDEO_HEADER *Video,long mVideo);
typedef int (BASEEXPORTFUNC *BaseGetFreeNum)(GBASE *base,BASE_INT *kod,
    long *num);
typedef int (BASEEXPORTFUNC *BaseObjectCount)(GBASE *base,BASE_INT *Code,
    int reg,long *Count);
typedef int (BASEEXPORTFUNC *BaseInitMultiQuery)(GBASE_MULTI_QUERY *Query,
    long Key,int Number);
typedef int (BASEEXPORTFUNC *BaseMultiReadObjectOpen)(GBASE *gbase,
    GBASE_MULTI_QUERY *zap,GBASE_MULTI_MKO *Filter,
    int Read,GBASE_OBJECT *Object);
typedef int (BASEEXPORTFUNC *BaseMultiReadObjectCont)(GBASE_MULTI_QUERY *zap,
    int Read,GBASE_OBJECT *Object);
typedef int (BASEEXPORTFUNC *BaseMultiReadObjectRetry)(GBASE_MULTI_QUERY *zap,
    int Read,GBASE_OBJECT *Object);
typedef int (BASEEXPORTFUNC *BaseMultiReadObjectRestore)(GBASE_MULTI_QUERY *zap);
typedef int (BASEEXPORTFUNC *BaseMultiReadObjectClose)(GBASE_MULTI_QUERY *zap);
typedef int (BASEEXPORTFUNC *BaseWriteObject)(GBASE *base,GBASE_OBJECT *Object,
    int Fun);
typedef int (BASEEXPORTFUNC *BaseDeleteObject)(GBASE *base,BASE_INT *code,
    unsigned long num,int reg);
typedef int (BASEEXPORTFUNC *BaseWriteNameClass)(GBASE *base,char *name);
typedef int (BASEEXPORTFUNC *BaseWriteName)(GBASE *base,char *name);
typedef int (BASEEXPORTFUNC *BaseWriteListkv)(GBASE *base,int reg,BASE_LISTKV *List);
typedef long (BASEEXPORTFUNC *BaseCheckPoint)(GBASE *base,int *RetCode);
typedef int (BASEEXPORTFUNC *BaseReadSvPr)(BASE_ZAP_SP *Query,
    GBASE_OBJECT *Object,int regsp,int reg1,BASE_INT *Select,
    unsigned long *Num,void * *pSP);
typedef int (BASEEXPORTFUNC *BaseGetAdrSvPrForNumber)(int Flag,
    GBASE_OBJECT *Object,unsigned long Number,BASE_SVPR_OLD **Adr);
typedef int (BASEEXPORTFUNC *BaseDeleteSvPr)(GBASE_OBJECT *Object,int reg,
    long nsp,void *adr);
typedef int (BASEEXPORTFUNC *BaseAddSvPr)(GBASE_OBJECT *Object,
    int reg,long nsp,void *adr,BASE_SVPR_OLD *pSP,void  * *pSPR,
    int CountItemsMet,int SizeItemMet);
typedef BASE_INT * (BASEEXPORTFUNC *BaseGetAdrSemSP)(void *AdrSP,int Flag);
typedef int (BASEEXPORTFUNC *BaseTransMKO)(char *Source,BASE_INT *MKO,
    int LengthMKO,int *ErrCode,int *ErrorSymbolNumber);
typedef int (BASEEXPORTFUNC *BaseNumbersCharToInt)(char *StrChar,
    BASE_INT *IntArray,int QNumbers,int QDigitals);
typedef int (BASEEXPORTFUNC *BaseDateDecode)(unsigned long abs_day,
    int *day,int *month,int *year);
typedef int (BASEEXPORTFUNC *BaseDateEncode)(    int day,int month,int year,
 unsigned long abs_day);
typedef double (BASEEXPORTFUNC *BaseObjectSquareAll)(int SizeItem,
 int TypeItem, int qPoints, void *Points,    double *s);
typedef int (BASEEXPORTFUNC *BaseCopyObject)(GBASE_OBJECT *ObjOut,
    GBASE_OBJECT *ObjIn,int Fun);
typedef int (BASEEXPORTFUNC *BaseCompCode)(BASE_INT *kod1,BASE_INT *kod2);
typedef int (BASEEXPORTFUNC *BaseSemCount)(BASE_INT *pSem,BASE_INT *kod_x,
    int reg,long *Count);
typedef int (BASEEXPORTFUNC *BaseReadFeature)(SEM_QUERY *Query,int Flag,
    int Reg,void *Adr,unsigned long NumberSvPr,FEATURE_CODE *CodeXar,
    BASE_XAR_VALUE *Value,BASE_FEATURE *Feature,int *Error);
typedef int (BASEEXPORTFUNC *BaseReadFeatureComp)(SEM_QUERY *Query,int Flag,
    int Reg,void *Adr,unsigned long NumberSvPr,FEATURE_CODE *CodeXar,
    int CountCodes,BASE_XAR_VALUE *Value,BASE_FEATURE *Feature,int *Error);
typedef int (BASEEXPORTFUNC *BaseDeleteFeature)(int Flag,void *Adr,
 unsigned long NumberSvPr,FEATURE_CODE *CodeXar,int *Error);
typedef int (BASEEXPORTFUNC *BaseDeleteFeatureComp)(int Flag,void *Adr,
 unsigned long NumberSvPr,FEATURE_CODE *CodeXar,int CountCodes,int *Error);
typedef int (BASEEXPORTFUNC *BaseAddFeature)(int Flag,void *Adr,
 unsigned long NumberSvPr,FEATURE_CODE *CodeInsert,BASE_FEATURE* Feature,
 BASE_XAR_VALUE *Value,int LengthSem,int *Error);
typedef int (BASEEXPORTFUNC *BaseAddFeatureComp)(int Flag,void *Adr,
 unsigned long NumberSvPr,FEATURE_CODE *CodeInsert,int N_Code,
 BASE_FEATURE* Feature,BASE_XAR_VALUE *Value,int LengthSem,int *Error);
typedef int (BASEEXPORTFUNC *BaseUpdateFeature)(int Flag,void *Adr,
 unsigned long NumberSvPr,FEATURE_CODE *CodeXar,BASE_FEATURE* Feature,
 BASE_XAR_VALUE *Value,int LengthSem,int *Error);
typedef int (BASEEXPORTFUNC *BaseUpdateFeatureComp)(int Flag,void *Adr,
 unsigned long NumberSvPr,FEATURE_CODE *CodeXar,int N_Code,
 BASE_FEATURE* Feature,BASE_XAR_VALUE *Value,
 int LengthSem,int *Error);
typedef int (BASEEXPORTFUNC *BaseDelMetric)(GBASE_OBJECT *Object,
 int CountPoints,int StartPoint,int CountItemsMet,int SizeItemMet);
typedef int (BASEEXPORTFUNC *BaseAddMetric)(GBASE_OBJECT *Object,
 int CountPoints,int StartPoint,int CountItemsMet,int SizeItemMet);
typedef int (BASEEXPORTFUNC *BaseUpdateMetric)(GBASE_OBJECT *Object,
 int CountPoints,void *NewMet,int StartPoint,int CountItemsMet,int SizeItemMet);
typedef int (BASEEXPORTFUNC *BaseObjectConvertCoord)(GBASE_OBJECT *ObjIn,
 int TypeItemMetIn,int SizeItemMetIn,double UnitMetIn,GBASE_OBJECT *ObjOut,
 int TypeItemMetOut,int SizeItemMetOut,double UnitMetOut);
typedef int (BASEEXPORTFUNC *BaseDeleteServiceObjects)(GBASE *base,
 int CountCodes,BASE_LIST_CODE *ListCodes);
typedef int (BASEEXPORTFUNC *BaseGetFormat)(void);
typedef int (BASEEXPORTFUNC *BaseReadConnectObjects)(GBASE *base,BASE_ZAP_SP *z,
    GBASE_OBJECT *obj,char *sl,int *err_sym,int Read,long Key,BASE_INT *mko,int lmko,
    int Fun,int r,GBASE_OBJECT *obj1,void  *  *AddrSVPR,int Flag);
typedef int (BASEEXPORTFUNC *BaseReadObjectList)(GBASE *base,GBASE_QUERY *zap,
    BASE_INT *Select,int Fun,int Read,GBASE_OBJECT *obj,int num);
typedef int (BASEEXPORTFUNC *BaseReadObjectKV)(GBASE *base,GBASE_QUERY *zap,
    int Fun,int *kv,int Read,BASE_INT *Select,GBASE_OBJECT *obj);
typedef int (BASEEXPORTFUNC *BaseReadObjectLKV)(GBASE *base,GBASE_QUERY *zap,
    BASE_INT *Select,BASE_INT *kodo,int Fun,int *kv,int Read,GBASE_OBJECT *Object);
typedef int (BASEEXPORTFUNC *BaseMultiReadObjectListOpen)(GBASE *gbase,
    GBASE_MULTI_QUERY *zap,GBASE_MULTI_MKO *Filter,
    int Read,GBASE_OBJECT *Object,int *NumberList);
typedef int (BASEEXPORTFUNC *BaseMultiReadObjectListCont)(GBASE_MULTI_QUERY *zap,
    int Read,GBASE_OBJECT *Object);
typedef int (BASEEXPORTFUNC *BaseMultiReadObjectListRetry)(GBASE_MULTI_QUERY *zap,
    int Read,GBASE_OBJECT *Object);
typedef int (BASEEXPORTFUNC *BaseAddPrToPoint)(GBASE_OBJECT *Object,
  int reg,int nsp,void *adr1,unsigned long NumberPoint,
        BASE_SVPR_OLD *pSP,void  * *pSPR,int CountItemsMet,int SizeItemMet);
typedef int (BASEEXPORTFUNC *BaseDeletePrNumber)(GBASE *gbase,BASE_INT *Code,
    unsigned long NumberObj,long NumberPr);
typedef int (BASEEXPORTFUNC *BaseMultiFindAnyObjectCoordIF)(int TypeFind,
 double X,double Y,GBASE *base,GBASE_MULTI_MKO *Filter,
 int CountNotFindCode, GBASE_NOT_FIND_OBJECT *NotFind,
 GBASE_FIND_ANY_OBJECT *FindObject);
typedef int (BASEEXPORTFUNC *BaseFindAnyObjectCoordIF)(int TypeFind,
 double X,double Y,void *Base,int CountFindCode,BASE_INT *ListFindCode,
 BASE_INT *Select,int CountNotFindCode,GBASE_NOT_FIND_OBJECT *NotFind,
 GBASE_FIND_ANY_OBJECT *FindObject);
typedef void (BASEEXPORTFUNC *BaseSetFindMaxDistance)(double MaxDistance);
typedef double (BASEEXPORTFUNC *BaseGetFindMaxDistance)(void);
typedef int (BASEEXPORTFUNC *BaseReadClassInfoObject)(int BaseFormat,GBASE_OBJECT *Object,
    char *Name,BASE_INT *Descriptor);

extern BaseCodeStr _BaseCodeStr;
extern BaseOpen _BaseOpen;
extern BaseClose _BaseClose;
extern BaseOpenTextError _BaseOpenTextError;
extern BaseSetFormat _BaseSetFormat;
extern BaseSetMetDescriptor _BaseSetMetDescriptor;
extern BasePackMode _BasePackMode;
extern BaseReadObject _BaseReadObject;
extern BaseReadNumbers _BaseReadNumbers;
extern BaseSetupQuery _BaseSetupQuery;
extern BaseCloseObject _BaseCloseObject;
extern BaseReadNameClass _BaseReadNameClass;
extern BaseReadName _BaseReadName;
extern BaseReadListkv _BaseReadListkv;
extern BaseReadPassword _BaseReadPassword;
extern BaseReadClassInfo _BaseReadClassInfo;
extern BaseInitQuery _BaseInitQuery;
extern BaseInitObject _BaseInitObject;
extern BaseGetFreeNum _BaseGetFreeNum;
extern BaseObjectCount _BaseObjectCount;
extern BaseInitMultiQuery _BaseInitMultiQuery;
extern BaseMultiReadObjectOpen _BaseMultiReadObjectOpen;
extern BaseMultiReadObjectCont _BaseMultiReadObjectCont;
extern BaseMultiReadObjectRetry _BaseMultiReadObjectRetry;
extern BaseMultiReadObjectRestore _BaseMultiReadObjectRestore;
extern BaseMultiReadObjectClose _BaseMultiReadObjectClose;
extern BaseWriteObject _BaseWriteObject;
extern BaseDeleteObject _BaseDeleteObject;
extern BaseWriteNameClass _BaseWriteNameClass;
extern BaseWriteName _BaseWriteName;
extern BaseWriteListkv _BaseWriteListkv;
extern BaseCheckPoint _BaseCheckPoint;
extern BaseReadSvPr _BaseReadSvPr;
extern BaseGetAdrSvPrForNumber _BaseGetAdrSvPrForNumber;
extern BaseDeleteSvPr _BaseDeleteSvPr;
extern BaseAddSvPr _BaseAddSvPr;
extern BaseGetAdrSemSP _BaseGetAdrSemSP;
extern BaseCodeStr _BaseCodeStr;
extern BaseTransMKO _BaseTransMKO;
extern BaseNumbersCharToInt _BaseNumbersCharToInt;
extern BaseDateEncode _BaseDateEncode;
extern BaseDateDecode _BaseDateDecode;
extern BaseObjectSquareAll _BaseObjectSquareAll;
extern BaseCopyObject _BaseCopyObject;
extern BaseCompCode _BaseCompCode;
extern BaseSemCount _BaseSemCount;
extern BaseReadFeature _BaseReadFeature;
extern BaseReadFeatureComp _BaseReadFeatureComp;
extern BaseDeleteFeature _BaseDeleteFeature;
extern BaseDeleteFeatureComp _BaseDeleteFeatureComp;
extern BaseAddFeature _BaseAddFeature;
extern BaseAddFeatureComp _BaseAddFeatureComp;
extern BaseUpdateFeature _BaseUpdateFeature;
extern BaseUpdateFeatureComp _BaseUpdateFeatureComp;
extern BaseDelMetric _BaseDelMetric;
extern BaseAddMetric _BaseAddMetric;
extern BaseUpdateMetric _BaseUpdateMetric;
extern BaseObjectConvertCoord _BaseObjectConvertCoord;
extern BaseDeleteServiceObjects _BaseDeleteServiceObjects;
extern BaseGetFormat _BaseGetFormat;
extern BaseReadConnectObjects _BaseReadConnectObjects;
extern BaseReadObjectList _BaseReadObjectList;
extern BaseReadObjectLKV _BaseReadObjectLKV;
extern BaseReadObjectKV _BaseReadObjectKV;
extern BaseMultiReadObjectListOpen _BaseMultiReadObjectListOpen;
extern BaseMultiReadObjectListCont _BaseMultiReadObjectListCont;
extern BaseMultiReadObjectListRetry _BaseMultiReadObjectListRetry;
extern BaseAddPrToPoint _BaseAddPrToPoint;
extern BaseDeletePrNumber _BaseDeletePrNumber;
extern BaseMultiFindAnyObjectCoordIF _BaseMultiFindAnyObjectCoordIF;
extern BaseFindAnyObjectCoordIF _BaseFindAnyObjectCoordIF;
extern BaseSetFindMaxDistance _BaseSetFindMaxDistance;
extern BaseGetFindMaxDistance _BaseGetFindMaxDistance;
extern BaseReadClassInfoObject _BaseReadClassInfoObject;


#endif
