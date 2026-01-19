/*****************************************************************//**
 * \file   EEPROM_25LC040A.h
 * \brief  Определение класса микросхемы 25LC040A.
 *	Документация по микросхеме https://docs.yandex.ru/docs/view?tm=1768404736&tld=ru&lang=en&name=DOC033229394.pdf&text=%D0%BC%D0%B8%D0%BA%D1%80%D0%BE%D1%81%D1%85%D0%B5%D0%BC%D0%B0%2025lc040a%20%D0%B4%D0%BE%D0%BA%D1%83%D0%BC%D0%B5%D0%BD%D1%82%D0%B0%D1%86%D0%B8%D1%8F&url=https%3A%2F%2Fstatic.chipdip.ru%2Flib%2F229%2FDOC033229394.pdf&lr=213&mime=pdf&l10n=ru&sign=aa6ba94a52d6d5383c91be3462e5ee02&keyno=0&serpParams=tm%3D1768404736%26tld%3Dru%26lang%3Den%26name%3DDOC033229394.pdf%26text%3D%25D0%25BC%25D0%25B8%25D0%25BA%25D1%2580%25D0%25BE%25D1%2581%25D1%2585%25D0%25B5%25D0%25BC%25D0%25B0%2B25lc040a%2B%25D0%25B4%25D0%25BE%25D0%25BA%25D1%2583%25D0%25BC%25D0%25B5%25D0%25BD%25D1%2582%25D0%25B0%25D1%2586%25D0%25B8%25D1%258F%26url%3Dhttps%253A%2F%2Fstatic.chipdip.ru%2Flib%2F229%2FDOC033229394.pdf%26lr%3D213%26mime%3Dpdf%26l10n%3Dru%26sign%3Daa6ba94a52d6d5383c91be3462e5ee02%26keyno%3D0
 * \author alexv
 * \date   January 2026
 *********************************************************************/
#pragma once
#include <cstdint>
#include "SPIDriver.h"
#include "MemoryDevice.h"

//EEPROM = Electrically Erasable Programmable Read-Only Memory

/**
 * \brief Класс для работы с микросхемой 25LC040A (память EEPROM) посредством драйвера 
 */
class EEPROM_25LC040A : public MemoryDevice {
private:

	static constexpr uint16_t MEMORY_SIZE = 512; ///< Размер памяти в байтах
	
	static constexpr uint8_t READ = 0x03;		///< Читает данные из памяти начиная с указанного адреса
	static constexpr uint8_t WRITE = 0x02;		///< Записывает данные, начиная с указанного адреса
	static constexpr uint8_t WRDI = 0x04;		///< Запрещает операции записи
	static constexpr uint8_t WREN = 0x06;		///< Разрешает операции записи
	static constexpr uint8_t RDSR = 0x05;		///< Чтение из статусного регистра
	static constexpr uint8_t WRSR = 0x01;		///< Запись в статусный регистр

	/**
	 * \brief Метод для ожидания завершения записи 
	 */
	void waitForWriteFinished();
public:

	/**
	 * \brief Конструктор класса 
	 * \param spi Ссылка на драйвер
	 */
	explicit EEPROM_25LC040A(SPIDriver& _spi) : MemoryDevice(_spi) {}

	/**
	 * \brief Читает один байт по указанному адресу
	 * \param addr Адрес для чтения
	 * \return Прочитанный байт
	 */
	uint8_t readByte(uint32_t addr) override;   

	/**
	 * \brief Читает последовательность байт, начиная с указанного адреса
	 * \param addr Адрес начала
	 * \param buff Буфер для записи
	 * \param length Размер буффера
	 */
	void readBytes(uint32_t addr, uint8_t* buff, size_t length) override;

	/**
	 * \brief Записывает один байт по указанному адресу
	 * \param addr Адрес для записи
	 * \param data Байт для записи
	 */
	void writeByte(uint32_t addr, uint8_t data) override;

	/**
	 * \brief Записывает последовательность байт по указанному адресу
	 * \param addr Адрес для записи
	 * \param buff Буфер с данными для записи
	 * \param length Размер буфера
	 */
	void writeBytes(uint32_t addr, const uint8_t* buff, size_t length) override;
};
