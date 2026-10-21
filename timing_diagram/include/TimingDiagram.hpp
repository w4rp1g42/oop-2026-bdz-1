#ifndef TIMINGDIAGRAM_H
#define TIMINGDIAGRAM_H

#include <vector>

#include "TernarySignal.hpp"

/**
 * @brief Класс временной диаграммы
 *
 * Последовательность отрезков TernarySignal, идущих друг за другом.
 */
class TimingDiagram {
    std::vector<TernarySignal> signals; ///< Последовательность отрезков сигнала
public:
    /**
     * @brief Пустой конструктор
     *
     * Создает пустую диаграмму
     */
    TimingDiagram() = default;

    /**
     * @brief Конструктор из состояния и длительности
     * @param state Состояние
     * @param duration Длительность, в тактах, больше 0
     * @throw std::logic_error Если duration == 0
     */
    TimingDiagram(TernarySignalState state, size_t duration) {
        signals.emplace_back(state, duration);
    }

    /**
     * @brief Конструктор из строки
     * @param signal_string Строка, состоящая из отрезков одинаковых символов '0', '1' или 'X'
     * @throw std::logic_error Если строка пустая
     * @throw std::invalid_argument Если неверный символ
     */
    explicit TimingDiagram(const std::string &signal_string);

    /**
     * @brief Заменяет диапазон [start, end) на содержимое other
     * @param other Другая диаграмма, ее длительность должна быть end - start
     * @param start Начало диапазона
     * @param end Конец диапазона
     * @throw std::invalid_argument Если end <= start
     * @throw std::logic_error Если other.getTotalDuration() != end - start
     * @throw std::logic_error Если end > getTotalDuration()
     */
    void replaceWithOther(const TimingDiagram &other, size_t start, size_t end);

    /**
     * @brief Добавляет other в конец текущей диаграммы
     * @param other Добавляемая диаграмма
     * @return Ссылка на текущий объект
     */
    TimingDiagram &operator +=(const TimingDiagram &other);

    /**
     * @brief Возвращает копию диаграммы, повторенную times раз
     * @param times Количество повторений, 0 дает пустую
     * @return Новая диаграмма
     */
    TimingDiagram operator *(size_t times) const;

    /**
     * @brief Сдвиг содержимого диаграммы влево на shift тактов
     * @param shift Величина сдвига
     * @return Новая сдвинутая диаграмма
     */
    TimingDiagram operator <<(size_t shift) const;

    /**
     * @brief Сдвиг содержимого диаграммы вправо на shift тактов
     * @param shift Величина сдвига
     * @return Новая сдвинутая диаграмма
     */
    TimingDiagram operator >>(size_t shift) const;

    /**
     * @brief Геттер для signals
     * @return Копия вектора сигналов
     */
    std::vector<TernarySignal> getSignals() const {
        return signals;
    }

    /**
     * @brief Суммарная длительность диаграммы
     * @return Сумма duration всех входящих сигналов
     */
    size_t getTotalDuration() const;

    /**
     * @brief Выводит диаграмму в поток
     * @param stream Поток вывода
     * @param diagram Диаграмма для вывода
     * @return Ссылка на переданный поток (для цепочек)
     * @throw std::logic_error Если состояние какого-либо входящего сигнала неверное
     */
    friend std::ostream &operator <<(std::ostream &stream, const TimingDiagram &diagram);

    /**
     * @brief Читает диаграмму из потока
     * @param stream Поток ввода
     * @param diagram Целевая диаграмма
     * @return Ссылка на переданный поток (для цепочек)
     *
     * При ошибке устанавливает std::ios::failbit и не изменяет diagram.
     */
    friend std::istream &operator >>(std::istream &stream, TimingDiagram &diagram);
};

#endif //TIMINGDIAGRAM_H
