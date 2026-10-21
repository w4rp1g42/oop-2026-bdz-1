#ifndef TERNARY_SIGNAL_H
#define TERNARY_SIGNAL_H

#include <stdexcept>
#include <string>

/**
 * @brief Состояние троичного сигнала
 */
enum TernarySignalState {
    SIGNAL_ZERO = 0, ///< Логический ноль, обозначается '_'
    SIGNAL_ONE = 1, ///< Логическая единица, обозначается '\''
    SIGNAL_UNDEFINED = 2 ///< Неопределенное состояние, обозначается '-'
};

/**
 * @brief Класс отрезка троичного сигнала
 *
 * Непрерывный участок сигнала с состоянием
 * и длительностью. Пустой сигнал (duration == 0) только
 * через конструктор по умолчанию.
 */
class TernarySignal {
    TernarySignalState state = SIGNAL_ZERO; ///< Состояние сигнала
    size_t duration = 0; ///< Длительность сигнала в тактах

    static char get_state_symbol(TernarySignalState raw_state);

    static bool is_valid_state(TernarySignalState raw_state);

public:
    /**
     * @brief Пустой конструктор
     *
     * Создает сигнал с duration == 0 и state == SIGNAL_ZERO.
     * Единственный способ получить пустой сигнал.
     */
    TernarySignal() = default;

    /**
     * @brief Инициализирующий конструктор
     * @param state Состояние сигнала
     * @param duration Длительность сигнала, должна быть больше 0
     * @throw std::logic_error Если duration == 0
     */
    TernarySignal(const TernarySignalState state, const size_t duration)
        : state(state), duration(duration) {
        if (duration == 0) throw std::logic_error("can not make signal with duration of 0");
    }

    /**
     * @brief Конструктор из строки
     * @param signal_string Строка вида "111", "0000", "XXXX"
     * @throw std::logic_error Если строка пуста
     * @throw std::invalid_argument Если первый символ не '0', '1' или 'X'
     *
     * Идет по одинаковым символам до первого отличного:
     * "11100" даст сигнал с state == SIGNAL_ONE и duration == 3.
     */
    explicit TernarySignal(const std::string &signal_string);

    /**
     * @brief Разделяет сигнал на две части в заданный момент
     * @param moment Момент разделения, в диапазоне (0, duration)
     * @return Вторая часть сигнала, первая часть остается в текущем объекте
     * @throw std::logic_error Если duration == 0
     * @throw std::logic_error Если moment == 0 или moment >= duration
     */
    TernarySignal split(size_t moment);

    /**
     * @brief Увеличивает длительность сигнала
     * @param increase_amount Величина увеличения длительности
     */
    void increase_duration(size_t increase_amount);

    /**
     * @brief Уменьшает длительность сигнала
     * @param decrease_amount Величина уменьшения, меньше duration
     * @throw std::logic_error Если duration == 0
     * @throw std::logic_error Если decrease_amount >= duration
     */
    void decrease_duration(size_t decrease_amount);

    /**
     * @brief Возвращает строковое представление сигнала
     * @return Строка из duration символов: '_' для SIGNAL_ZERO,
     *         '\'' для SIGNAL_ONE, '-' для SIGNAL_UNDEFINED;
     *         пустая строка для пустого сигнала
     * @throw std::logic_error Если state не является допустимым значением TernarySignalState
     */
    std::string draw() const;

    /**
     * @brief Геттер для state
     * @return Состояние сигнала в виде TernarySignalState
     */
    TernarySignalState getState() const { return state; }

    /**
     * @brief Геттер для duration
     * @return Длительность сигнала
     */
    size_t getDuration() const { return duration; }

    /**
     * @brief Сеттер для state
     * @param new_state Новый state
     * @throw std::logic_error Если new_state не является допустимым
     *        значением TernarySignalState
     */
    void setState(const TernarySignalState new_state) {
        if (!is_valid_state(new_state)) throw std::logic_error("invalid state");
        state = new_state;
    }

    /**
     * @brief Сеттер для duration
     * @param new_duration Новая длительность, больше 0
     * @throw std::logic_error Если new_duration == 0
     */
    void setDuration(const size_t new_duration) {
        if (new_duration == 0) throw std::logic_error("can not make signal with duration of 0");
        duration = new_duration;
    }

    /**
     * @brief Выводит сигнал в поток
     * @param stream Поток вывода
     * @param signal Сигнал для вывода
     * @return Ссылка на переданный поток (для цепочек)
     * @throw std::logic_error Если state недопустим
     */
    friend std::ostream &operator <<(std::ostream &stream, const TernarySignal &signal);

    /**
     * @brief Читает сигнал из потока
     * @param stream Поток ввода
     * @param signal Сигнал для ввода
     * @return Ссылка на переданный поток (для цепочек)
     *
     * При ошибке устанавливает std::ios::failbit и не изменяет signal.
     * Можно смешивать записи одного состояния:
     * "111''''" соответствует SIGNAL_ONE длительностью 7.
     */
    friend std::istream &operator >>(std::istream &stream, TernarySignal &signal);
};

#endif //TERNARY_SIGNAL_H
