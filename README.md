# Shining Saturn 360

Emulador de Sega Saturn para Xbox 360 (RGH/JTAG).

## Estado técnico — v0.39b

- Testes de regressão no Ubuntu: aprovados na execução do GitHub Actions.
- Toolchain libXenon: verificada.
- ELF32 PowerPC big-endian: compilado e validado.
- **Artefato `default.xex`: gerado e aprovado nas verificações estruturais** na execução [#19 do GitHub Actions](https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38208936616).
- [Obter o artefato `ShiningSaturn360-default-XEX`](https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38208936616/artifacts/11728084585). O ZIP inclui `default.xex`, `default-xex-sha256.txt` e `xex-tools-sha256.txt`.
- SHA-256 do `default.xex`: `74fd21700f4a5e16e7695e67380400d5b7eab88913f32e0b3f2b8fcb53cc537e`.
- Tamanho do XEX: 397.312 bytes; imagem: 393.216 bytes; ponto de entrada reportado pelo empacotador: `0x00005500`.

## Limitações atuais

O `default.xex` é um bootstrap técnico. A criação e as verificações estruturais do arquivo **não** comprovam que ele inicializa em um Xbox 360 real, nem que o núcleo de emulação Sega Saturn esteja funcional ou execute jogos. O empacotador também alertou que uma seção gravável compartilha uma página com código; esse alinhamento precisa ser corrigido e validado antes de considerar o XEX pronto para testes no console.

A versão permanece **v0.39b** até que haja outro marco técnico verificável. O artefato do GitHub Actions tem retenção limitada.

## Compilação na nuvem

Abra o workflow **Shining Saturn 360 - Xenon audit and build**:
https://github.com/MeiKinosaki/Shining-Saturn-360/actions
