# lamp_control

## Управляемый по сети фонарь. 
Фонарь принимает команды управления от сервера. Предполагается, что реализация сервера уже существует.

Фонарь и сервер общаются по протоколу, работающему поверх соединения TCP.

Протокол управления фонарем устроен следующим образом. Для изменения состояния фонаря сервер передает ему команду управления.
Все команды кодируются в виде TLV ([wikipedia](http://en.wikipedia.org/wiki/Type-length-value)), при
этом поле type имеет размер 1 байт, поле length — 2 байта и поле value — length байт.
Все данные передаются по сети в Big Endian.

Протокол версии 1 описывает три команды:
- **ON** (включить фонарь): type = 0x12, length = 0
- **OFF** (выключить фонарь): type = 0x13, length = 0
- **COLOR** (сменить цвет): type = 0x20, length = 3, value интерпретируется как новый цвет фонаря в RGB.
Предполагается, что в будущих версиях протокола могут появляться новые команды, однако структура TLV останется неизменной.

Реализация фонаря удовлетворяет следующим требованиям:

1. При запуске через командную строку передаются хост и порт (по умолчанию хост 127.0.0.1, порт 9999), 
осуществляется соединение по TCP и после этого отрабатывается протокол управления.

2. При получении данных от сервера фонарь собирает целые команды (type + length + value) и, если type известен, 
обрабатывает команду, иначе молча ее игнорирует.

3. При получении команды ON фонарь включается.

4. При получении команды OFF фонарь выключается.

5. При получении команды COLOR фонарь меняет цвет.

6. При завершении работы фонарь корректно закрывает соединение с сервером.
