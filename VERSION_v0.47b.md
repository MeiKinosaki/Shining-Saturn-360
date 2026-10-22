# Shining Saturn 360 — v0.47b

## Marco
Incremento validado do núcleo interpretador SH-2 com cálculo de endereço PC-relativo (`MOVA`) e teste de bits entre registradores (`TST Rm,Rn`).

## Alterações incluídas
- Implementado `MOVA @(disp,PC),R0`, com base PC+4 alinhada a 4 bytes e deslocamento multiplicado por quatro.
- Implementado `TST Rm,Rn`, atualizando somente o bit T de SR conforme o resultado lógico AND e preservando os registradores de origem.
- Criados testes específicos para MOVA e TST, integrados a `tests/run-sh2-core-tests.sh`.
- Os commits correspondentes passaram pelo GitHub Actions:
  - MOVA / suíte: https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/39766978254
  - TST / suíte: https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/39767699164

## Estado de validação
- **CI automatizado das alterações MOVA e TST: aprovado.**
- Isso não comprova que todos os testes, o pacote `default.xex` ou a imagem ELF mais recente foram validados juntos em um único artefato de release.
- Inicialização em Xbox 360 físico: não comprovada.
- Compatibilidade com jogos comerciais: não comprovada.

## Limitações conhecidas
- O interpretador SH-2 permanece parcial; ainda faltam instruções, exceções, interrupções, privilégios e temporização fiel.
- MMIO e vários dispositivos continuam incompletos; VDP1/VDP2, SCU/DMA, CD Block, SCSP e SMPC ainda não formam um sistema completo.
- A versão é um marco incremental de código e testes, não uma alegação de emulação jogável.

## Próximo marco — v0.48b
Priorizar testes e implementação de exceções/interrupções e a integração entre execução SH-2, barramento e ciclo principal. Promover somente após evidência de CI para as alterações do marco.
