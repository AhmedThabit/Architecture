///*
// * storage.h
// *
// * Simple storage layer for managing a phonebook and audio recordings
// * on a W25Q32 serial NOR flash.  The design reserves the first 4 kB
// * sector of flash for a header structure containing phone numbers,
// * offsets and lengths for up to 16 recordings, and a CRC for
// * integrity.  Subsequent sectors are used to store the raw audio
// * payloads.  The API exposes functions to initialise the flash,
// * save/retrieve phone numbers and save/read recordings.  All
// * operations update the header and persist it back to flash.
// */
//
//#ifndef STORAGE_H
//#define STORAGE_H
//
//#include <stdint.h>
//#include <stddef.h>
//#include <stdbool.h>
//
///* Maximum number of phone book entries and recordings supported */
//#define STORAGE_MAX_ENTRIES   16
//
///* Maximum length of a phone number string (including null terminator).
// * 16 bytes allows up to 15 digits plus a terminating '\0'.
// */
//#define STORAGE_MAX_NUMBER_LEN 16
//
///* Initialise the storage system.  This must be called once at
// * start‑up before any other storage API is used.  It initialises
// * the underlying SPI flash and loads (or creates) the header.
// */
//void Storage_Init(void);
//
///* Save a phone number string into the given slot (0–15).  The
// * supplied string should be a null‑terminated string containing
// * only digits (no spaces or separators).  If it exceeds
// * STORAGE_MAX_NUMBER_LEN‑1 characters it will be truncated.
// * Returns true on success, false if the slot index is out of range
// * or if the header could not be written.
// */
//bool Storage_SavePhoneNumber(int slot, const char *number);
//
///* Retrieve a phone number from the given slot.  The caller must
// * provide a buffer of at least STORAGE_MAX_NUMBER_LEN bytes.  On
// * success the number string is copied into the buffer and the
// * function returns true.  If the slot index is invalid or the
// * stored number is empty, false is returned and the buffer is
// * unchanged.
// */
//bool Storage_GetPhoneNumber(int slot, char *outNumber);
//
///* Save a recording into the given slot (0–15).  The payload is
// * written into flash at the next available offset after any
// * previously stored recordings.  The header is updated with the
// * recording's offset and length.  Returns true on success, false
// * if the slot index is invalid, if there is not enough space in
// * flash for the data or if any write operation fails.
// */
//bool Storage_SaveRecording(int slot, const uint8_t *data, size_t len);
//
///* Read a recording from the given slot.  The caller must pass a
// * buffer large enough to hold the recording.  On return *outLen
// * contains the actual number of bytes read.  Returns true on
// * success, false if the slot index is invalid or if no recording
// * exists in that slot.
// */
//bool Storage_ReadRecording(int slot, uint8_t *outData, size_t *outLen);
//
//#endif /* STORAGE_H */