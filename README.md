# Shining Saturn 360

Emulador de Sega Saturn para Xbox 360 (RGH/JTAG).

## Estado técnico — v0.40b

- Testes de regressão no Ubuntu: aprovados.
- Toolchain libXenon: verificada.
- ELF32 PowerPC big-endian: compilado e validado.
- **`default.xex`: gerado e aprovado nas verificações estruturais na execução [#24 do GitHub Actions](https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38210300632).
- [Obter o artefato `ShiningSaturn360-default-XEX`](https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38210300632/artifacts/11729282280). O ZIP contém `default.xex`, `default-xex-sha256.txt` e `xex-tools-sha256.txt`. O artefato expira em 26 de outubro de 2026.
- SHA-256 de `default.xex`: `87e448b99226a43fea5acfea7e2abd407ce308f8c6f839dec692d1cc39204b1c`.
- Tamanho do XEX: 593.920 bytes; imagem: 589.824 bytes; base: `0x00001000`; ponto de entrada: `0x00005500`.
- O empacotador reportou 4 páginas `CODE` seguidas de 5 páginas `RWDATA`; a verificação estrutural XEX2 passou sem o aviso anterior de página compartilhada.

## Limitações atuais

O `default.xex` continua sendo um bootstrap técnico. A geração do arquivo e as verificações estruturais não comprovam que ele inicializa em um Xbox 360 real, nem que o núcleo de emulação Sega Saturn esteja funcional ou execute jogos. A inicialização no console ainda não foi testada.

A versão `v0.40b` registra o marco de empacotamento e alinhamento estrutural; não representa uma versão funcional completa do emulador. O artefato do GitHub Actions tem retenção limitada.

## Compilação na nuvem

Abra o workflow **Shining Saturn 360 - Xenon audit and build**:
https://github.com/MeiKinosaki/Shining-Saturn-360/actions
