# Artifact Manifest

All sizes are raw bytes. SHA-256 hashes were calculated from the original files available in the working session. The 0.1–0.5 packages and exact uploaded text are nested inside the directly committed 0.6 archive; their hashes remain listed for independent integrity checks.

| File | Bytes | SHA-256 | Repository role |
|---|---:|---|---|
| `Pasted text.txt` | 23617 | `32cbdc48c36590ee558a6931e8a3b1f4998e608007d67b1e0644ee0a39fddc34` | Exact uploaded master-prompt source |
| `TheVeil_CausalSpine_0_1.zip` | 13146 | `513b26cb3a4e04734fef25849702d3ccfd78cdc452dc0732b9b2865af90a5d17` | Historical 0.1 source and README |
| `TheVeil_RoundPhase_0_2.zip` | 28508 | `dbf7e78f131ef81e02071ee3d51a53800e022817287079c91070e0671d497a68` | Historical 0.2 cumulative source, tags, tests and integration docs |
| `TheVeil_MemoryProvenance_0_3.zip` | 57920 | `90de7cb91f9c22224a8c26624f43e16ad91cc8a22832200645647a71262dc0ba` | Historical 0.3 cumulative source and docs |
| `TheVeil_ClaimVoting_0_4.zip` | 76978 | `910bd6c964fec91fb46e0f56b60a4444f1a39bb1af0878e3fb17fdfe7384bd77` | Historical 0.4 cumulative source and docs |
| `TheVeil_ContextArchive_0_5.zip` | 81802 | `39193adff86760216e37eb9f7a9a1cbfcdeab9fcab969d665c01ce76fe077908` | First complete documentation archive |
| `TheVeil_FullProjectArchive_0_6.zip` | 365467 | `835662918997e9cbc88dca396745d8bceae771f3e8121760fdcbcddd9fc32797` | Exhaustive public project-context and artifact archive |

The directly committed 0.6 archive contains the audited project documents, exact original prompt source and historical packages 0.1–0.5. It does not recursively include itself.

## Integrity rules

- Current repository source is authoritative over historical ZIP contents.
- ZIP files must remain byte-for-byte unchanged once committed.
- New generated packages require a new version and manifest entry.
- No copyrighted reference assets or soundtrack files are included.
