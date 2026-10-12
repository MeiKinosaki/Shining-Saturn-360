# Shining Saturn 360

Emulador de Sega Saturn para Xbox 360 (RGH/JTAG).

## Estado técnico — v0.39b

- Testes de regressão no Ubuntu: aprovados na execução do GitHub Actions.
- Toolchain libXenon: verificada.
- Bootstrap ELF32 PowerPC big-endian: compilado e validado na execução anterior.
- O artefato ELF32 pode ser obtido na página de artefatos da execução:
  https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/38206634114
- Workflow atual também contém uma etapa condicional para converter o ELF32 em `default.xex`, caso o ambiente disponibilize um conversor compatível.

## Limitações atuais

O ELF32 compilado é um bootstrap técnico, não uma versão funcional completa do emulador Sega Saturn. Ainda não há confirmação de inicialização no Xbox 360 nem de execução de jogos. O `default.xex` só será considerado disponível depois que o workflow gerar e publicar esse artefato com sucesso.

A versão permanece **v0.39b** até haver outro avanço técnico verificável.

## Compilação na nuvem

Abra GitHub Actions e execute o workflow **Shining Saturn 360 - Xenon audit and build**:
https://github.com/MeiKinosaki/Shining-Saturn-360/actions
