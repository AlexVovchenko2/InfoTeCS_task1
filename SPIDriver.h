/*****************************************************************//**
 * \file   SPIDriver.h
 * \brief  Реализация виртуального класса драйвера
 * 
 * \author alexv
 * \date   January 2026
 *********************************************************************/

#pragma once
#include <cstdint>
/**
 * \brief Абстрактный интерфейс драйвера. Предполагается, что реальный будет наследоваться от него.
 */
class SPIDriver{
//SPI работает в полнодуплексном режиме: данные передаются и принимаются одновременно. 
public:
	/**
	 * \brief Передаёт байт по SPI и возвращает принимаемый байт
	 * \param data Байт для передачи
	 * \return Принимаемый байт
	 */
	virtual uint8_t transfer(const uint8_t data) = 0;

	/**
	 * \brief Передаёт массив байтов по SPI
	 * \param transmitBuffer Указатель на массив байтов для передачи
	 * \param recieveBuffer Указатель на массив принимаемых байтов 
	 * \param length Количество байтов (размер буффера)
	 * \return 
	 */
	virtual void transfer(const uint8_t* transmitBuffer, uint8_t* recieveBuffer, size_t length) = 0;

	/**
	 * \brief Устанавливает состояние микросхемы
	 * \param active Если true, то переводится в активное состояние (начало сессии)
	 */
	virtual void enableChip(bool active) = 0;
};
