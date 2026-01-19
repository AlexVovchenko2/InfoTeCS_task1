/*****************************************************************//**
 * \file   MemoryDevice.h
 * \brief  Реализация виртуального класса микросхемы
 * 
 * \author alexv
 * \date   January 2026
 *********************************************************************/
#pragma once
#include <cstdint>
#include "SPIDriver.h"

/**
 * \brief Базовый класс для реализации микросхем с разными типами последовательной памяти (SAM).
 */
class MemoryDevice {
protected:
	/**
	 * \brief Указатель на используемый драйвер
	 */
	SPIDriver& spi;

	explicit MemoryDevice(SPIDriver& _spi) : spi(_spi) {}
public:

	/**
	 * \brief Читает один байт по указанному адресу
	 * \param addr Адрес для чтения
	 * \return Прочитанный байт
	 */
	virtual uint8_t readByte(uint32_t addr) = 0;   //32 бита под адрес точно хватит 

	/**
	 * \brief Читает последовательность байт, начиная с указанного адреса
	 * \param addr Адрес начала
	 * \param buff Буфер для записи
	 * \param length Размер буффера
	 */
	virtual void readBytes(uint32_t addr, uint8_t* buff, size_t length) = 0;

	/**
	 * \brief Записывает один байт по указанному адресу 
	 * \param addr Адрес для записи
	 * \param data Байт для записи
	 */
	virtual void writeByte(uint32_t addr, uint8_t data) = 0;

	/**
	 * \brief Записывает последовательность байт по указанному адресу
	 * \param addr Адрес для записи
	 * \param buff Буфер с данными для записи
	 * \param length Размер буфера
	 */
	virtual void writeBytes(uint32_t addr, const uint8_t* buff, size_t length) = 0;
};
