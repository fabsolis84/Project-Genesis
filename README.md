# Project Genesis

Project Genesis is a historical integration/support repository for the UnitLink hardware and firmware program.

## Authority Boundary

Active UnitLink engineering authority is no longer owned here.

- Architecture, hardware records, decisions, BOM control, GPIO assignments, relay/switching records, and hardware standards: `fabsolis84/UnitLink-Knowledge-Base`
- UnitLink-owned Apollo and Atlas firmware: `fabsolis84/UnitLink-Firmware`
- Kernel, provisioning, broker integration, and canonical MQTT bridge: `fabsolis84/unitlink-os`

The legacy `hardware/` tree in this repository is historical/reference-only and must not be treated as the controlled Apollo or Atlas hardware library.

## Historical Components

### Firmware
- UnitLink-Indoor (Apollo)
- UnitLink-Outdoor (Atlas)

### Shared Libraries
- UnitLinkWireless
- UnitLinkMQTT
- UnitLinkPacket

### Server
- MQTT Broker
- Python Worker
- API Services

### Hardware
- Historical Apollo placeholders/reference
- Historical Atlas placeholders/reference
- Historical PCB/schematic placeholders/reference

### Documentation
- Historical protocol specifications
- Historical hardware documentation
- Historical API documentation
- Historical development milestones

## Development Philosophy

- Modular
- Shared codebase
- Version controlled from day one
- Build first, optimize second

For current work, begin with `fabsolis84/UnitLink-Knowledge-Base/docs/00-start-here/PROJECT_BOOTSTRAP.md`.