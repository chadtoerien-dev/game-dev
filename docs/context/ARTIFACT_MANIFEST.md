# Artifact Manifest

All sizes are raw bytes. SHA-256 hashes were calculated from the original files available in the working session.

| File | Bytes | SHA-256 | Coverage |
|---|---:|---|---|
| `Pasted text.txt` | 23617 | `32cbdc48c36590ee558a6931e8a3b1f4998e608007d67b1e0644ee0a39fddc34` | Exact uploaded master-prompt source; also preserved verbatim in ordered Markdown parts |
| `TheVeil_CausalSpine_0_1.zip` | 13146 | `513b26cb3a4e04734fef25849702d3ccfd78cdc452dc0732b9b2865af90a5d17` | Historical 0.1 source and README; cumulative content represented by current source and milestone docs |
| `TheVeil_RoundPhase_0_2.zip` | 28508 | `dbf7e78f131ef81e02071ee3d51a53800e022817287079c91070e0671d497a68` | Historical 0.2 cumulative source and docs |
| `TheVeil_MemoryProvenance_0_3.zip` | 57920 | `90de7cb91f9c22224a8c26624f43e16ad91cc8a22832200645647a71262dc0ba` | Historical 0.3 cumulative source and docs |
| `TheVeil_ClaimVoting_0_4.zip` | 76978 | `910bd6c964fec91fb46e0f56b60a4444f1a39bb1af0878e3fb17fdfe7384bd77` | Historical 0.4 cumulative source and docs |
| `TheVeil_ContextArchive_0_5.zip` | 81802 | `39193adff86760216e37eb9f7a9a1cbfcdeab9fcab969d665c01ce76fe077908` | First documentation archive; unpacked content is in `docs/` |
| `TheVeil_FullProjectArchive_0_6.zip` | 365467 | `835662918997e9cbc88dca396745d8bceae771f3e8121760fdcbcddd9fc32797` | Verified portable handoff archive containing the exact original prompt, packages 0.1–0.5 and audited context |

## Repository treatment

The authoritative source and documentation are committed unpacked. The verified 0.6 ZIP is supplied through the project handoff because the GitHub connector could not guarantee byte-identical binary transfer. The failed test binary was removed before merge.

## Integrity rules

- Current repository source is authoritative over historical ZIP contents.
- Published hashes must not be changed without generating a new version.
- No copyrighted reference assets or soundtrack files are included.
