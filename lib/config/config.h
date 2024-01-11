#pragma once

#include "elrs_eeprom.h"

// CONFIG_MAGIC is ORed with CONFIG_VERSION in the version field
#define TX_BACKPACK_CONFIG_MAGIC    (0b01 << 30)
#define VRX_BACKPACK_CONFIG_MAGIC   (0b10 << 30)

#define TX_BACKPACK_CONFIG_VERSION      3
#define VRX_BACKPACK_CONFIG_VERSION     3

#if defined(TARGET_TX_BACKPACK)
typedef struct {
    uint32_t    version;
    bool        startWiFi;
    char        ssid[33];
    char        password[65];
    uint8_t     address[6];
} tx_backpack_config_t;

class TxBackpackConfig
{
public:
    void Load();
    void Commit();

    // Getters
    bool     IsModified() const { return m_modified; }
    bool     GetStartWiFiOnBoot() { return m_config.startWiFi; }
    char    *GetSSID() { return m_config.ssid; }
    char    *GetPassword() { return m_config.password; }
    uint8_t *GetGroupAddress() { return m_config.address; }

    // Setters
    void SetStorageProvider(ELRS_EEPROM *eeprom);
    void SetDefaults();
    void SetStartWiFiOnBoot(bool startWifi);
    void SetSSID(const char *ssid);
    void SetPassword(const char *ssid);
    void SetGroupAddress(const uint8_t address[6]);

private:
    tx_backpack_config_t    m_config;
    ELRS_EEPROM             *m_eeprom;
    bool                    m_modified;
};

extern TxBackpackConfig config;

#endif

///////////////////////////////////////////////////

#if defined(TARGET_VRX_BACKPACK)
typedef struct {
    uint32_t    version;
    uint8_t     bootCount;
    bool        startWiFi;
    char        ssid[33];
    char        password[65];
    uint8_t     address[6];

#if defined(AAT_BACKPACK)
    struct __attribute__((packed)) tagAatConfig {
        uint8_t     satelliteHomeMin;   // minimum number of satellites to establish home
        uint8_t     servoSmooth;    // 0-9 for min smoothing to most smoothing
        uint8_t     centerDir;      // Direction servo points at center position 0=N 1=E 2=S 3=W
        uint8_t     project;        // FUTURE: 0=none, 1=projectAzim, 2=projectElev, 3=projectBoth
        uint8_t     units;          // FUTURE: 0=meters, anything else=also meters :-D
        uint8_t     servoMode;      // FUTURE: reserved to declare 2:1, 180+flip servo, or 180 clipped
                                    // Also maybe invertAzim / invertElev servo bit or just swap low/high
        struct tagServoEndoint {
            uint16_t low;
            uint16_t high;
        } servoEndpoints[2];        // us endpoints for servos
    } aat;

    struct __attribute__((packed)) tagVbatConfig {
        uint16_t scale;
        int16_t offset;
    } vbat;
#endif
} vrx_backpack_config_t;

class VrxBackpackConfig
{
public:
    void Load();
    void Commit();

    // Getters
    bool     IsModified() const { return m_modified; }
    uint8_t  GetBootCount() { return m_config.bootCount; }
    bool     GetStartWiFiOnBoot() { return m_config.startWiFi; }
    char    *GetSSID() { return m_config.ssid; }
    char    *GetPassword() { return m_config.password; }
    uint8_t *GetGroupAddress() { return m_config.address; }

    // Setters
    void SetStorageProvider(ELRS_EEPROM *eeprom);
    void SetDefaults();
    void SetBootCount(uint8_t count);
    void SetStartWiFiOnBoot(bool startWifi);
    void SetSSID(const char *ssid);
    void SetPassword(const char *ssid);
    void SetGroupAddress(const uint8_t address[6]);

#if defined(AAT_BACKPACK)
    uint8_t GetAatSatelliteHomeMin() const { return m_config.aat.satelliteHomeMin; }
    uint8_t GetAatServoSmooth() const { return m_config.aat.servoSmooth; }
    void SetAatServoSmooth(uint8_t val);
    uint8_t GetAatProject() const { return m_config.aat.project; }
    uint8_t GetAatCenterDir() const { return m_config.aat.centerDir; }
    void SetAatCenterDir(uint8_t val);
    uint16_t GetAatServoLow(uint8_t idx) const { return m_config.aat.servoEndpoints[idx].low; }
    void SetAatServoLow(uint8_t idx, uint16_t val);
    uint16_t GetAatServoHigh(uint8_t idx) const { return m_config.aat.servoEndpoints[idx].high; }
    void SetAatServoHigh(uint8_t idx, uint16_t val);

    uint16_t GetVbatScale() const { return m_config.vbat.scale; }
    void SetVbatScale(uint16_t val);
    int16_t GetVbatOffset() const { return m_config.vbat.offset; }
    void SetVbatOffset(int16_t val);
#endif

private:
    vrx_backpack_config_t   m_config;
    ELRS_EEPROM             *m_eeprom;
    bool                    m_modified;
};

extern VrxBackpackConfig config;

#endif
