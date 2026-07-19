## Scope of this file

This file governs **Claude Code** sessions in this repository only. It is additive: it does not modify, supersede, or govern the ChatGPT Project Instructions or any document in `fabsolis84/UnitLink-Knowledge-Base`. Those remain exactly as written.

## What this repo is

Project-Genesis is the FORGE-owned embedded/hardware/Hub implementation repo for the UnitLink platform: Apollo (indoor controller), Atlas (outdoor controller), UnitLink Hub, shared libraries, server components, and hardware design files. It implements approved architecture — it does not define it.

The permanent source of truth for architecture and decisions is `fabsolis84/UnitLink-Knowledge-Base`. This repo's `CHANGELOG.md`, `FIRMWARE_VERSION.md`, and the `firmware` submodule are implementation evidence, not architecture.

## Required reading before implementation work

From `fabsolis84/UnitLink-Knowledge-Base`, read in order:

1. `docs/00-start-here/PROJECT_BOOTSTRAP.md`
2. `docs/00-start-here/ENGINEERING_CONSTITUTION.md`
3. `docs/00-start-here/roles/FORGE.md`
4. `docs/00-start-here/CURRENT_WORKING_STATE.md`
5. `docs/07-development/sprint-0-genesis-foundation.md` and `docs/07-development/project-genesis-milestones.md`
6. Relevant decisions in `docs/02-decisions/` — in particular DEC-0039 (repository-first firmware development), DEC-0040 (Project Genesis firmware architecture), and DEC-0045 through DEC-0051 (Hub/Apollo/Atlas topology, Apollo Lite, sensor rules, demo/production endpoint separation)

## Implementation rules

- Build only approved architecture; do not redesign an approved decision without explicit user approval (`ENGINEERING_CONSTITUTION.md`, Implementation Rules).
- The `firmware` directory/submodule is the active Apollo/Atlas implementation evidence — validate locally before treating a milestone as complete.
- Per DEC-0051, the frozen Mango demo path (`https://admin.mangohvac.com/wp-json/ul/v1/ingest`) must not be modified as a side effect of production Apollo work; keep `demo_mango` and `apollo_production` environment profiles explicit and separate.
- When finishing a task, report changed files, validation performed, and commit hash(es), matching the FORGE reporting convention in the ULKB.

## Claude-specific tooling note

Claude Code has direct GitHub access via MCP tools in this session. The ChatGPT-specific "connector discovery" workaround documented in the ULKB's `ROLE_EXECUTION_ENGINE.md` (staged tool loading, false "GitHub unavailable" claims) does not apply here and should be ignored — if a GitHub tool call fails, that failure is the actual signal.

## Non-goals

This file does not modify or govern the ChatGPT Project Instructions or any content in `fabsolis84/UnitLink-Knowledge-Base`. Documentation and architecture changes still belong in that repository, through its own SCRIBE process.
