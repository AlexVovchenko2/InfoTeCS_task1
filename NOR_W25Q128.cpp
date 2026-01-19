/*****************************************************************//**
 * \file   NOR_W25Q128.cpp
 * \brief  –еализаци€ методов класса NOR_W25Q128
 * 
 * \author alexv
 * \date   January 2026
 *********************************************************************/
#include "NOR_W25Q128.h"

uint8_t NOR_W25Q128::readByte(uint32_t addr) {
	if (addr >= MEMORY_SIZE) return 0;  // в идеальном мире здесь должно быть исключение, но мне лень

	spi.enableChip(true);
	spi.transfer(READ);

	// нетрудно заметить, что дл€ W25Q128 нужно 24 бит под адрес => будем передавать 3 байта
	spi.transfer(static_cast<uint8_t>((addr >> 16) & 0xFF));
	spi.transfer(static_cast<uint8_t>((addr >> 8) & 0xFF));
	spi.transfer(static_cast<uint8_t>(addr & 0xFF));

	uint8_t temp = spi.transfer(0x00);
	spi.enableChip(false);
	return temp;
}

void NOR_W25Q128::readBytes(uint32_t addr, uint8_t* buff, size_t length) {
	if (length == 0 || !buff || addr >= MEMORY_SIZE) return;
	if (length + addr >= MEMORY_SIZE) length = MEMORY_SIZE - addr;

	spi.enableChip(true);

	spi.transfer(READ);

	spi.transfer(static_cast<uint8_t>((addr >> 16) & 0xFF));
	spi.transfer(static_cast<uint8_t>((addr >> 8) & 0xFF));
	spi.transfer(static_cast<uint8_t>(addr & 0xFF));

	for (size_t i = 0; i < length; i++) {
		buff[i] = spi.transfer(0x00);
	}

	spi.enableChip(false);
}

void NOR_W25Q128::writeByte(uint32_t addr, uint8_t data) {
	if (addr >= MEMORY_SIZE) return;

	spi.enableChip(true);
	spi.transfer(WRITE_ENABLE);
	spi.enableChip(false);

	spi.enableChip(true);
	spi.transfer(PAGE_PROG);

	spi.transfer(static_cast<uint8_t>((addr >> 16) & 0xFF));
	spi.transfer(static_cast<uint8_t>((addr >> 8) & 0xFF));
	spi.transfer(static_cast<uint8_t>(addr & 0xFF));

	spi.transfer(data);
	spi.enableChip(false);

	waitForWriteFinished();
}

void NOR_W25Q128::writeBytes(uint32_t addr, const uint8_t* buff, size_t length) {
	if (length == 0 || !buff || addr >= MEMORY_SIZE) return;
	if (length + addr >= MEMORY_SIZE) length = MEMORY_SIZE - addr;


	size_t writtenBytes = 0;
	// так как запись возможна в пределах одной страницы(page), то будем разбивать буфер на части при необходимости
	while (writtenBytes < length) {
		size_t currentPageStart = ((addr + writtenBytes) / PAGE_SIZE) * PAGE_SIZE;
		size_t offset = addr + writtenBytes - currentPageStart;
		size_t bytesToWrite = (length - writtenBytes < PAGE_SIZE - offset) ?
			length - writtenBytes : PAGE_SIZE - offset;

		spi.enableChip(true);
		spi.transfer(WRITE_ENABLE);
		spi.enableChip(false);

		spi.enableChip(true);
		spi.transfer(PAGE_PROG);

		size_t currentAddr = addr + writtenBytes;
		spi.transfer(static_cast<uint8_t>((currentAddr >> 16) & 0xFF));
		spi.transfer(static_cast<uint8_t>((currentAddr >> 8) & 0xFF));
		spi.transfer(static_cast<uint8_t>(currentAddr & 0xFF));


		for (size_t i = 0; i < bytesToWrite; i++) {
			spi.transfer(buff[writtenBytes + i]);
		}

		spi.enableChip(false);
		waitForWriteFinished();

		writtenBytes += bytesToWrite;
	}
}

void NOR_W25Q128::eraseSector(uint32_t addr) {
	if (addr >= MEMORY_SIZE || addr % SECTOR_SIZE != 0) return;

	spi.enableChip(true);
	spi.transfer(WRITE_ENABLE);
	spi.enableChip(false);

	spi.enableChip(true);
	spi.transfer(SECTOR_ERASE);

	spi.transfer(static_cast<uint8_t>((addr >> 16) & 0xFF));
	spi.transfer(static_cast<uint8_t>((addr >> 8) & 0xFF));
	spi.transfer(static_cast<uint8_t>(addr & 0xFF));

	spi.enableChip(false);

	waitForWriteFinished();
}

void NOR_W25Q128::eraseChip() {
	spi.enableChip(true);
	spi.transfer(WRITE_ENABLE);
	spi.enableChip(false);

	spi.enableChip(true);
	spi.transfer(CHIP_ERASE);
	spi.enableChip(false);

	waitForWriteFinished();  // будет долго
}

bool NOR_W25Q128::isBusy() {
	spi.enableChip(true);
	spi.transfer(READ_STATUS_REGISTER1);
	uint8_t status = spi.transfer(0x00);
	return (status & 0x01) != 0;	// нас интересует 1-й бит статусного регистра. 
									// ≈сли он 1 - значит Write In Progress, 0 - свободно
}

uint32_t NOR_W25Q128::getJEDECID() {
	spi.enableChip(true);
	spi.transfer(READ_JEDEC_ID);
	uint8_t manufactureID = spi.transfer(0x00);
	uint8_t memoryType = spi.transfer(0x00);
	uint8_t memoryCapacity = spi.transfer(0x00);
	spi.enableChip(false);

	return (static_cast<uint32_t>(manufactureID) << 16 |
		static_cast<uint32_t>(memoryType) << 8 |
		static_cast<uint32_t>(memoryCapacity));
}

void NOR_W25Q128::waitForWriteFinished() {
	const uint32_t MAX_TIMEOUT = 120000;  // 120 сек

	uint32_t time = MAX_TIMEOUT;

	while (isBusy() && --time);
}
