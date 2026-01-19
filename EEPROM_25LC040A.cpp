/*****************************************************************//**
 * \file   EEPROM_25LC040A.cpp
 * \brief  Реализация методов из класса EEPROM_25LC040A
 * 
 * \author alexv
 * \date   January 2026
 *********************************************************************/
#include "EEPROM_25LC040A.h"

void EEPROM_25LC040A::waitForWriteFinished() {
	uint8_t status;
	do {
		spi.enableChip(true);
		spi.transfer(RDSR);
		status = spi.transfer(0x00);
		spi.enableChip(false);
	} while (status & 0x01);  // нас интересует 1-й бит статусного регистра. 
							  // Если он 1 - значит Write In Progress, 0 - свободно
}

uint8_t EEPROM_25LC040A::readByte(uint32_t addr) {
	if (addr >= MEMORY_SIZE) return 0;   // в идеальном мире здесь должно быть исключение, но мне лень

	spi.enableChip(true);
	spi.transfer(READ);

	// нетрудно заметить, что для 25LC040A нам хватит 16 бит под адрес => будем передавать 2 байта
	spi.transfer(static_cast<uint8_t>((addr >> 8) & 0xFF));		//передаём старший байт
	spi.transfer(static_cast<uint8_t>(addr & 0xFF));			//передаём младший байт

	uint8_t temp = spi.transfer(0x00);
	spi.enableChip(false);
	return temp;
}

void EEPROM_25LC040A::readBytes(uint32_t addr, uint8_t* buff, size_t length) {
	if (length == 0 || !buff || addr >= MEMORY_SIZE) return;
	if (length + addr >= MEMORY_SIZE) length = MEMORY_SIZE - addr;

	spi.enableChip(true);

	spi.transfer(READ);

	spi.transfer(static_cast<uint8_t>((addr >> 8) & 0xFF));		//передаём старший байт
	spi.transfer(static_cast<uint8_t>(addr & 0xFF));			//передаём младший байт

	for (size_t i = 0; i < length; i++) {
		buff[i] = spi.transfer(0x00);
	}

	spi.enableChip(false);
}

void EEPROM_25LC040A::writeByte(uint32_t addr, uint8_t data) {
	if (addr >= MEMORY_SIZE) return;

	spi.enableChip(true);
	spi.transfer(WREN);    //по умолчанию запись запрещена, надо разрешить
	spi.enableChip(false);

	spi.enableChip(true);
	spi.transfer(WRITE);

	spi.transfer(static_cast<uint8_t>((addr >> 8) & 0xFF));		//передаём старший байт
	spi.transfer(static_cast<uint8_t>(addr & 0xFF));			//передаём младший байт

	spi.transfer(data);
	spi.enableChip(false);

	waitForWriteFinished();
}

void EEPROM_25LC040A::writeBytes(uint32_t addr, const uint8_t* buff, size_t length) {
	if (length == 0 || !buff || addr >= MEMORY_SIZE) return;
	if (length + addr >= MEMORY_SIZE) length = MEMORY_SIZE - addr;

	spi.enableChip(true);
	spi.transfer(WREN);    //по умолчанию запись запрещена, надо разрешить
	spi.enableChip(false);

	spi.enableChip(true);
	spi.transfer(WRITE);

	spi.transfer(static_cast<uint8_t>((addr >> 8) & 0xFF));		//передаём старший байт
	spi.transfer(static_cast<uint8_t>(addr & 0xFF));			//передаём младший байт


	for (size_t i = 0; i < length; i++) {
		spi.transfer(buff[i]);
	}

	spi.enableChip(false);

	waitForWriteFinished();
}
