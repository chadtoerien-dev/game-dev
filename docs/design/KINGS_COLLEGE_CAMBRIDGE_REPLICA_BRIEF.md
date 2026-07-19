# King’s College Cambridge Replica Brief

Last updated: 19 July 2026

## Approved direction

The environment target is a maximum-fidelity reconstruction of King’s College, Cambridge. Within each approved production footprint, use one Unreal Unit per centimetre and reproduce source-verified positions, proportions, materials and sightlines as closely as production, access, performance and rights allow.

“One-to-one” means one-to-one spatial scale and evidence-led architectural fidelity. It does not mean claiming accuracy where measurements are unavailable, copying third-party photos or maps into production assets, reproducing protected branding, or expanding the current vertical slice into the full College before the gameplay gate is stable.

The Veil’s characters, factions, dialogue, missions, UI, music and social fiction remain original. Any use of the King’s name, crest, heraldry, signage, choir recordings or other brand/cultural material requires separate clearance.

## Current production footprint

The vertical slice remains one compact connected location. Its first exterior reference footprint should establish:

- Front Court’s overall enclosure and principal approach;
- the Wilkins screen and gatehouse as the arrival threshold;
- the Chapel exterior as the dominant orientation landmark;
- the court-facing ranges and fountain as scale and navigation anchors;
- the transition towards the lawn, Meadow and River Cam as the quieter edge of the play space.

Interior rooms required by the game may use real spatial relationships only where authorised plans or surveys support them. Unverified, inaccessible or gameplay-compressed interiors must be labelled `AUTHORED_INTERPRETATION`, not presented as an exact reconstruction.

The full College is not a Milestone 0.5 deliverable. Expansion is gated behind the complete-round and performance milestones.

## Gameplay-zone allocation

This table aligns the vertical-slice route with the real-world basis without inventing unsupported accuracy.

| Game zone | King’s basis | Initial fidelity | Production rule |
|---|---|---|---|
| Principal approach and entrance hall | Wilkins screen and gatehouse threshold | `VISUALLY_REFERENCED` | Preserve exterior scale and arrival sightline; the playable interior needs authorised evidence before promotion. |
| Courtyard | Front Court, fountain and surrounding ranges | `SOURCE_TRIANGULATED` target | This is the primary orientation and challenge space; keep Chapel massing and court proportions legible. |
| Dining room | Hall within the court-facing range | `VISUALLY_REFERENCED` | Use authorised interior sources only; gameplay furniture and clearance changes enter the deviation register. |
| Drawing/social room | Old Lodge or a court-facing common-room interpretation | `AUTHORED_INTERPRETATION` | Do not imply an exact private interior without plans or survey access. |
| Library/archive route | Wilkins library context plus an original secured archive | mixed | Library-facing geometry may follow evidence; the secured archive, mechanisms and mission route remain original game content. |
| Tribunal chamber | Original ceremonial chamber located within an approved range | `AUTHORED_INTERPRETATION` | Do not present the Chapel as the game’s exile chamber without an explicit cultural, narrative and rights decision. |
| Guest corridor and bedrooms | Approved residential-range footprint | `AUTHORED_INTERPRETATION` | Retain one-to-one exterior envelope where verified; private-room plans require authorisation. |
| Kitchen and service route | Evidence-led service access where available | `AUTHORED_INTERPRETATION` | Preserve believable structure while prioritising playable circulation and accessibility. |
| Underground storage and secret passage | Original game spaces beneath a verified footprint | `AUTHORED_INTERPRETATION` | Never market invented subterranean routes as real features of King’s College. |
| Meadow and river edge | lawn, Meadow and River Cam transition | `VISUALLY_REFERENCED` | Use as the quiet exterior boundary and long-sightline contrast, subject to the approved footprint. |

## Fidelity classes

| Class | Meaning | Permitted production claim |
|---|---|---|
| `SURVEY_VERIFIED` | Geometry is based on an authorised survey, licensed measured plan or project-owned measurement capture. | One-to-one measured reconstruction of the recorded elements. |
| `SOURCE_TRIANGULATED` | Geometry is reconstructed from at least two compatible authoritative sources but lacks a project survey. | High-fidelity reconstruction; dimensions remain provisional. |
| `VISUALLY_REFERENCED` | Publicly visible form is modelled from lawful observation and descriptive sources without a complete measurement set. | Visually faithful approximation. |
| `AUTHORED_INTERPRETATION` | Spatial or decorative detail is invented for gameplay, access, safety or missing evidence. | Original production interpretation; never call it exact. |

Every environment module must carry one fidelity class in the environment source register. A module cannot be promoted without evidence.

## Source and rights register

This register tracks research authority, not permission to copy source media into the game.

| ID | Source | Intended use | Current status |
|---|---|---|---|
| `KCC-OFFICIAL-HISTORY` | [King’s College — History of the Chapel](https://www.kings.cam.ac.uk/history-chapel) | chronology, architectural vocabulary and landmark context | Public research reference; no media reuse granted |
| `KCC-OFFICIAL-CHAPEL` | [King’s College Chapel](https://www.kings.cam.ac.uk/kings-college-chapel) | official identity and visitor-context cross-check | Public research reference; no media reuse granted |
| `KCC-OFFICIAL-GROUNDS` | [Greener King’s](https://www.kings.cam.ac.uk/greener-kings) | grounds, Meadow and biodiversity context | Public research reference; no media reuse granted |
| `KCC-FILMING` | [Filming and Photography at King’s](https://www.kings.cam.ac.uk/filming-and-photography-kings-college) | access and capture-planning requirements | Human permission checkpoint; application required before project capture on private property |
| `KCC-COPYRIGHT` | [Guidance on Copyright, Reprography and Publishing](https://www.kings.cam.ac.uk/guidance-copyright-reprography-and-publishing) | archive, plan, map and photograph reproduction boundary | Human rights checkpoint before reproduction or commercial publication |
| `HE-1139003` | [Historic England list entry 1139003](https://historicengland.org.uk/listing/the-list/list-entry/1139003) | Chapel designation and authoritative architectural description | Descriptive research reference; observe linked map/data terms |

Before adding a photograph, scan, map, plan, texture, crest or recording to source control, record its owner, creator, date, licence/permission, allowed uses, attribution requirement, expiry and repository location. “Available online” is not a licence.

## Measurement and coordinate standard

- Unreal scale: `1 uu = 1 cm`.
- Project origin: a documented, stable survey point chosen during the blockout approval session.
- North: record true/grid north and Unreal yaw offset in the environment manifest.
- Elevation: use one documented datum; do not silently zero individual buildings.
- Dimensions: store source ID, measured value, uncertainty and fidelity class.
- Photogrammetry: use only project-owned or explicitly licensed capture; retain consent/access paperwork and raw-capture provenance outside the packaged game.
- Modular tolerances: record intentional seams, collision offsets and Nanite simplification separately from architectural dimensions.

## Deviation register

Every deliberate departure from the verified source must record:

| Field | Required value |
|---|---|
| Element | stable module or room identifier |
| Source baseline | source IDs and expected measurement/appearance |
| Production change | exact spatial or visual deviation |
| Reason | gameplay, accessibility, collision, camera, performance, rights or incomplete evidence |
| Player impact | navigation, sightline, stealth, social staging or none |
| Approval | Chad and date |
| Review trigger | evidence, playtest or performance condition that reopens it |

Gameplay clearance and readability may override literal geometry, but the deviation must be visible in this register rather than becoming accidental drift.

## Non-human work before the location Editor pass

1. Maintain the source/rights register and assign fidelity classes.
2. Build a measurement manifest from authorised dimensions with uncertainty fields.
3. Define modular grid, origin, north and elevation datum.
4. Produce an exterior blockout plan for the bounded vertical-slice route.
5. Generate automated scale, naming, collision-channel, LOD/Nanite and source-metadata checks.
6. Prepare a screenshot/capture shot list that compares the same camera positions against authorised references.
7. Keep all unverified interiors visibly labelled until plans or surveys are licensed.

## Human checkpoints

Chad must approve or obtain:

1. the exact phase-one footprint and whether real-world naming appears in the commercial game;
2. permission for any on-site project photography, filming, scanning or measurement;
3. licences for archive records, measured plans, maps and third-party photographs used beyond research;
4. any use of the King’s name, crest, heraldry, signage, choir or recordings;
5. the blockout origin, north, elevation datum and first deviation register;
6. visual fidelity in Editor and the final comparison capture.

This production brief is an evidence and workflow control, not legal advice. Commercial release requires a rights review based on the assets and claims actually used.
