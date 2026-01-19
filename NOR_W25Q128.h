/*****************************************************************//**
 * \file   NOR_W25Q128.h
 * \brief  Определение класса микросхемы W25Q128
 * Документация по микросхеме https://docs.yandex.ru/docs/view?tm=1768503521&tld=ru&lang=en&name=DOC001093213.pdf&text=%D0%BC%D0%B8%D0%BA%D1%80%D0%BE%D1%81%D1%85%D0%B5%D0%BC%D0%B0%20W25Q128%20%D0%B4%D0%BE%D0%BA%D1%83%D0%BC%D0%B5%D0%BD%D1%82%D0%B0%D1%86%D0%B8%D1%8F&url=https%3A%2F%2Fstatic.chipdip.ru%2Flib%2F093%2FDOC001093213.pdf&lr=213&mime=pdf&l10n=ru&sign=6e27a7bf202d7a4a51b0764e18b2e46a&keyno=0&serpParams=tm%3D1768503521%26tld%3Dru%26lang%3Den%26name%3DDOC001093213.pdf%26text%3D%25D0%25BC%25D0%25B8%25D0%25BA%25D1%2580%25D0%25BE%25D1%2581%25D1%2585%25D0%25B5%25D0%25BC%25D0%25B0%2BW25Q128%2B%25D0%25B4%25D0%25BE%25D0%25BA%25D1%2583%25D0%25BC%25D0%25B5%25D0%25BD%25D1%2582%25D0%25B0%25D1%2586%25D0%25B8%25D1%258F%26url%3Dhttps%253A%2F%2Fstatic.chipdip.ru%2Flib%2F093%2FDOC001093213.pdf%26lr%3D213%26mime%3Dpdf%26l10n%3Dru%26sign%3D6e27a7bf202d7a4a51b0764e18b2e46a%26keyno%3D0
 * \author alexv
 * \date   January 2026
 *********************************************************************/
#pragma once
#include "MemoryDevice.h"

/**
 * \brief Класс для работы с микросхемой W25Q128 (память NOR) посредством драйвера
 */
class NOR_W25Q128 : public MemoryDevice {
private:

	static constexpr uint32_t MEMORY_SIZE = 16 * 1024 * 1024;	///< Размер памяти: 128 Мбит = 16 Мбайт
	static constexpr size_t PAGE_SIZE = 256;					///< Размер программируемой страницы - 256 Байт
	static constexpr size_t SECTOR_SIZE = 4096;					///< Размер сектора - 4 Кбайт
	
	static constexpr uint8_t READ = 0x03;						///< Позволяет считать последовательно один или более байт данных из памяти
	static constexpr uint8_t FAST_READ = 0x0B;					///< Работает на более высокой частоте чем READ и требует передачи фиктивного байта
	static constexpr uint8_t PAGE_PROG = 0x02;					///< Позволяет перепрограммировать от 1 до 256 байт очищенной памяти
	static constexpr uint8_t SECTOR_ERASE = 0x20;				///< Устанавливает всю память внутри 4-Кбайтного сектора в очищенное состояние
	static constexpr uint8_t CHIP_ERASE = 0xC7;					///< Устанавливает всю память устройства в очищенное состояние
	static constexpr uint8_t WRITE_ENABLE = 0x06;				///< Устанавливает бит "Запись разрешена" в статусном регистре в состояние 1
	static constexpr uint8_t READ_STATUS_REGISTER1 = 0x05;		///< Делает статусный регистр доступным для чтения
	static constexpr uint8_t READ_JEDEC_ID = 0x9F;				///< Информация по микросхеме: ID производителя, тип памяти, ёмкость (под каждый пункт 1 байт).

	/**
	 * \brief Метод для ожидания завершения записи
	 */
	void waitForWriteFinished();

public:

	/**
	 * \brief Конструктор класса
	 * \param spi Ссылка на драйвер
	 */
	explicit NOR_W25Q128(SPIDriver& _spi) : MemoryDevice(_spi) {}


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

	/**
	 * \brief Очищает сектор
	 * \param addr Адрес, соответствующий границе сектора
	 */
	void eraseSector(uint32_t addr);

	/**
	 * \brief Очищает весь чип
	 */
	void eraseChip();

	/**
	 * \brief Проверяет завершена ли текущая инструкция
	 * \return true - если устройство занято
	 */
	bool isBusy();

	/**
	 * \brief Получает информацию об устройстве
	 * \return ID в виде 4-байтного числа => старший байт всегда 0
	 */
	uint32_t getJEDECID();
};
