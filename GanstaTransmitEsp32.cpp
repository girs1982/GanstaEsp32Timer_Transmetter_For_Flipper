#include "GanstaTransmitEsp32.h"

// Статическая переменная для хранения указателя на объект
static GanstaTransmitEsp32* _instance = nullptr;
// Конструктор класса
GanstaTransmitEsp32::GanstaTransmitEsp32(int inputPin)
    : _inputPin(inputPin), _repeatCount(0), _busy(false), _currentAction(0), _totalActions(0) {
    _timer = NULL;
}
// Инициализация библиотеки
void GanstaTransmitEsp32::begin() {
    pinMode(_inputPin, OUTPUT);
    digitalWrite(_inputPin, LOW);
    setupTimer();
}
// Настройка таймера
void GanstaTransmitEsp32::setupTimer() {
    _instance = this;  // Сохраняем указатель на текущий объект
    _timer = timerBegin(1, 80, true);  // Таймер 1, делитель 80 (1 мкс)
    timerAttachInterrupt(_timer, &onTimerStatic, true);  // Указываем функцию обработчик
    timerAlarmWrite(_timer, 1000, true);  // 1 мс
}



// Статическая функция обработчик прерывания
void IRAM_ATTR GanstaTransmitEsp32::onTimerStatic() {
    if (_instance != nullptr) {
        _instance->onTimer();  // Вызов нестатического метода onTimer
    }
}
// Основная логика таймера
void IRAM_ATTR GanstaTransmitEsp32::onTimer() {
    if (_currentAction < _totalActions) {
        _busy = true;
        digitalWrite(_inputPin, _encoder[_currentAction].state);
        timerAlarmWrite(_timer, _encoder[_currentAction].duration, true);
        _currentAction++;
    } else {
        if (_repeatCount > 0) {
            _currentAction = 0;
            _repeatCount--;
        } else {
            timerAlarmDisable(_timer);
            _busy = false;
            digitalWrite(_inputPin, LOW);
        }
    }
}

// Добавление действия в очередь
void GanstaTransmitEsp32::addEncoderAction(bool state, uint32_t duration) {
    if (_totalActions < MAX_ACTIONS) {
        _encoder[_totalActions].state = state;
        _encoder[_totalActions].duration = duration;
        _totalActions++;
    }
}

// Отправка пакета данных с указанием количества повторов
void GanstaTransmitEsp32::sendPacketStarline(byte* data, int length, int repeats) {
    if (_busy) {
        // Если передача идет, пропускаем выполнение
        return;
    }
    _repeatCount = repeats;
    _totalActions = 0;
    _currentAction = 0;
    // Заполняем массив с действиями для преамбулы
    for (int i = 6; i > 0; i--) {  // Преамбула
        addEncoderAction(true, 1000);
        addEncoderAction(false, 1000);
    }
    // Преобразование данных в последовательность действий
    for (int i = 0; i < length; i++) {
        byte currentByte = data[i];
        for (int j = 7; j >= 0; j--) {  // От 7 до 0 битов
            bool bitState = currentByte & (1 << j);
            if (!bitState) {
                addEncoderAction(true, 250);
                addEncoderAction(false, 250);
            } else {
                addEncoderAction(true, 500);
                addEncoderAction(false, 500);
            }
        }
    }
    // Добавляем в конце посылки дополнительные импульсы HIGH и LOW
    addEncoderAction(true, 1000);  // Пин в HIGH, задержка 1000 мкс
    addEncoderAction(false, 1000); // Пин в LOW, задержка 1000 мкс

    // Запускаем таймер с первым действием
    if (_totalActions > 0) {
        _busy = true;  // Устанавливаем флаг busy
        timerAlarmWrite(_timer, _encoder[0].duration, true);
        timerAlarmEnable(_timer);  // Включаем таймер
    }
}
// Проверка занятости передатчика
bool GanstaTransmitEsp32::isBusy() {
    return _busy;
}
