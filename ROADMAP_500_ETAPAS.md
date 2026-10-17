# Shining Saturn 360 — plano de avanço em 500 etapas

**Versão oficial mantida:** v0.45b até os critérios de aceitação do próximo marco serem satisfeitos.  
**Propósito:** trilha de trabalho verificável, sem saltar versões nem declarar etapas planejadas como concluídas.  
**Regra de conclusão:** marcar uma etapa como concluída somente quando houver alteração real no repositório ou teste/evidência correspondente. CI verde não prova execução em Xbox 360 físico nem compatibilidade com jogos comerciais.

## Como acompanhar
- Cada número abaixo é uma etapa de trabalho, não uma afirmação de que já foi executada.
- Atualizar status e links de commits à medida que cada etapa for realmente implementada e validada.
- Promover para v0.46b somente depois de revisar testes SH-2, build ELF32, empacotamento `default.xex` e notas de versão.
- A ordem de trabalho é sequencial; os cinco blocos cobrem as principais áreas do emulador.

## Bloco 1 — CPU SH-2, registradores, instruções e exceções
001. **decodificação de opcode** — documentar requisitos e referências técnicas.
001. **decodificação de opcode** — definir interfaces e invariantes do subsistema.
001. **decodificação de opcode** — implementar o caminho básico funcional.
001. **decodificação de opcode** — tratar limites, estados inválidos e erros.
001. **decodificação de opcode** — cobrir operações normais com testes unitários.
001. **decodificação de opcode** — adicionar casos extremos e regressões.
001. **decodificação de opcode** — integrar com o barramento e o scheduler.
001. **decodificação de opcode** — medir custo e corrigir gargalos sem alterar a semântica.
001. **decodificação de opcode** — registrar diagnóstico, limitações e evidências.
001. **decodificação de opcode** — executar validação de aceitação antes de marcar concluído.
002. **aritmética inteira** — documentar requisitos e referências técnicas.
002. **aritmética inteira** — definir interfaces e invariantes do subsistema.
002. **aritmética inteira** — implementar o caminho básico funcional.
002. **aritmética inteira** — tratar limites, estados inválidos e erros.
002. **aritmética inteira** — cobrir operações normais com testes unitários.
002. **aritmética inteira** — adicionar casos extremos e regressões.
002. **aritmética inteira** — integrar com o barramento e o scheduler.
002. **aritmética inteira** — medir custo e corrigir gargalos sem alterar a semântica.
002. **aritmética inteira** — registrar diagnóstico, limitações e evidências.
002. **aritmética inteira** — executar validação de aceitação antes de marcar concluído.
003. **lógica e comparações** — documentar requisitos e referências técnicas.
003. **lógica e comparações** — definir interfaces e invariantes do subsistema.
003. **lógica e comparações** — implementar o caminho básico funcional.
003. **lógica e comparações** — tratar limites, estados inválidos e erros.
003. **lógica e comparações** — cobrir operações normais com testes unitários.
003. **lógica e comparações** — adicionar casos extremos e regressões.
003. **lógica e comparações** — integrar com o barramento e o scheduler.
003. **lógica e comparações** — medir custo e corrigir gargalos sem alterar a semântica.
003. **lógica e comparações** — registrar diagnóstico, limitações e evidências.
003. **lógica e comparações** — executar validação de aceitação antes de marcar concluído.
004. **deslocamentos e rotações** — documentar requisitos e referências técnicas.
004. **deslocamentos e rotações** — definir interfaces e invariantes do subsistema.
004. **deslocamentos e rotações** — implementar o caminho básico funcional.
004. **deslocamentos e rotações** — tratar limites, estados inválidos e erros.
004. **deslocamentos e rotações** — cobrir operações normais com testes unitários.
004. **deslocamentos e rotações** — adicionar casos extremos e regressões.
004. **deslocamentos e rotações** — integrar com o barramento e o scheduler.
004. **deslocamentos e rotações** — medir custo e corrigir gargalos sem alterar a semântica.
004. **deslocamentos e rotações** — registrar diagnóstico, limitações e evidências.
004. **deslocamentos e rotações** — executar validação de aceitação antes de marcar concluído.
005. **multiplicação e divisão** — documentar requisitos e referências técnicas.
005. **multiplicação e divisão** — definir interfaces e invariantes do subsistema.
005. **multiplicação e divisão** — implementar o caminho básico funcional.
005. **multiplicação e divisão** — tratar limites, estados inválidos e erros.
005. **multiplicação e divisão** — cobrir operações normais com testes unitários.
005. **multiplicação e divisão** — adicionar casos extremos e regressões.
005. **multiplicação e divisão** — integrar com o barramento e o scheduler.
005. **multiplicação e divisão** — medir custo e corrigir gargalos sem alterar a semântica.
005. **multiplicação e divisão** — registrar diagnóstico, limitações e evidências.
005. **multiplicação e divisão** — executar validação de aceitação antes de marcar concluído.
006. **acessos à memória** — documentar requisitos e referências técnicas.
006. **acessos à memória** — definir interfaces e invariantes do subsistema.
006. **acessos à memória** — implementar o caminho básico funcional.
006. **acessos à memória** — tratar limites, estados inválidos e erros.
006. **acessos à memória** — cobrir operações normais com testes unitários.
006. **acessos à memória** — adicionar casos extremos e regressões.
006. **acessos à memória** — integrar com o barramento e o scheduler.
006. **acessos à memória** — medir custo e corrigir gargalos sem alterar a semântica.
006. **acessos à memória** — registrar diagnóstico, limitações e evidências.
006. **acessos à memória** — executar validação de aceitação antes de marcar concluído.
007. **desvios e delay slots** — documentar requisitos e referências técnicas.
007. **desvios e delay slots** — definir interfaces e invariantes do subsistema.
007. **desvios e delay slots** — implementar o caminho básico funcional.
007. **desvios e delay slots** — tratar limites, estados inválidos e erros.
007. **desvios e delay slots** — cobrir operações normais com testes unitários.
007. **desvios e delay slots** — adicionar casos extremos e regressões.
007. **desvios e delay slots** — integrar com o barramento e o scheduler.
007. **desvios e delay slots** — medir custo e corrigir gargalos sem alterar a semântica.
007. **desvios e delay slots** — registrar diagnóstico, limitações e evidências.
007. **desvios e delay slots** — executar validação de aceitação antes de marcar concluído.
008. **SR/PR/GBR/VBR/MACH/MACL** — documentar requisitos e referências técnicas.
008. **SR/PR/GBR/VBR/MACH/MACL** — definir interfaces e invariantes do subsistema.
008. **SR/PR/GBR/VBR/MACH/MACL** — implementar o caminho básico funcional.
008. **SR/PR/GBR/VBR/MACH/MACL** — tratar limites, estados inválidos e erros.
008. **SR/PR/GBR/VBR/MACH/MACL** — cobrir operações normais com testes unitários.
008. **SR/PR/GBR/VBR/MACH/MACL** — adicionar casos extremos e regressões.
008. **SR/PR/GBR/VBR/MACH/MACL** — integrar com o barramento e o scheduler.
008. **SR/PR/GBR/VBR/MACH/MACL** — medir custo e corrigir gargalos sem alterar a semântica.
008. **SR/PR/GBR/VBR/MACH/MACL** — registrar diagnóstico, limitações e evidências.
008. **SR/PR/GBR/VBR/MACH/MACL** — executar validação de aceitação antes de marcar concluído.
009. **exceções e interrupções** — documentar requisitos e referências técnicas.
009. **exceções e interrupções** — definir interfaces e invariantes do subsistema.
009. **exceções e interrupções** — implementar o caminho básico funcional.
009. **exceções e interrupções** — tratar limites, estados inválidos e erros.
009. **exceções e interrupções** — cobrir operações normais com testes unitários.
009. **exceções e interrupções** — adicionar casos extremos e regressões.
009. **exceções e interrupções** — integrar com o barramento e o scheduler.
009. **exceções e interrupções** — medir custo e corrigir gargalos sem alterar a semântica.
009. **exceções e interrupções** — registrar diagnóstico, limitações e evidências.
009. **exceções e interrupções** — executar validação de aceitação antes de marcar concluído.
010. **temporização e regressões** — documentar requisitos e referências técnicas.
010. **temporização e regressões** — definir interfaces e invariantes do subsistema.
010. **temporização e regressões** — implementar o caminho básico funcional.
010. **temporização e regressões** — tratar limites, estados inválidos e erros.
010. **temporização e regressões** — cobrir operações normais com testes unitários.
010. **temporização e regressões** — adicionar casos extremos e regressões.
010. **temporização e regressões** — integrar com o barramento e o scheduler.
010. **temporização e regressões** — medir custo e corrigir gargalos sem alterar a semântica.
010. **temporização e regressões** — registrar diagnóstico, limitações e evidências.
010. **temporização e regressões** — executar validação de aceitação antes de marcar concluído.
## Bloco 1 — Memória Saturn, barramento, MMIO e SCU/DMA
011. **mapa de memória real** — documentar requisitos e referências técnicas.
011. **mapa de memória real** — definir interfaces e invariantes do subsistema.
011. **mapa de memória real** — implementar o caminho básico funcional.
011. **mapa de memória real** — tratar limites, estados inválidos e erros.
011. **mapa de memória real** — cobrir operações normais com testes unitários.
011. **mapa de memória real** — adicionar casos extremos e regressões.
011. **mapa de memória real** — integrar com o barramento e o scheduler.
011. **mapa de memória real** — medir custo e corrigir gargalos sem alterar a semântica.
011. **mapa de memória real** — registrar diagnóstico, limitações e evidências.
011. **mapa de memória real** — executar validação de aceitação antes de marcar concluído.
012. **work RAM baixa e alta** — documentar requisitos e referências técnicas.
012. **work RAM baixa e alta** — definir interfaces e invariantes do subsistema.
012. **work RAM baixa e alta** — implementar o caminho básico funcional.
012. **work RAM baixa e alta** — tratar limites, estados inválidos e erros.
012. **work RAM baixa e alta** — cobrir operações normais com testes unitários.
012. **work RAM baixa e alta** — adicionar casos extremos e regressões.
012. **work RAM baixa e alta** — integrar com o barramento e o scheduler.
012. **work RAM baixa e alta** — medir custo e corrigir gargalos sem alterar a semântica.
012. **work RAM baixa e alta** — registrar diagnóstico, limitações e evidências.
012. **work RAM baixa e alta** — executar validação de aceitação antes de marcar concluído.
013. **ROM/BIOS e aliases** — documentar requisitos e referências técnicas.
013. **ROM/BIOS e aliases** — definir interfaces e invariantes do subsistema.
013. **ROM/BIOS e aliases** — implementar o caminho básico funcional.
013. **ROM/BIOS e aliases** — tratar limites, estados inválidos e erros.
013. **ROM/BIOS e aliases** — cobrir operações normais com testes unitários.
013. **ROM/BIOS e aliases** — adicionar casos extremos e regressões.
013. **ROM/BIOS e aliases** — integrar com o barramento e o scheduler.
013. **ROM/BIOS e aliases** — medir custo e corrigir gargalos sem alterar a semântica.
013. **ROM/BIOS e aliases** — registrar diagnóstico, limitações e evidências.
013. **ROM/BIOS e aliases** — executar validação de aceitação antes de marcar concluído.
014. **endianness e alinhamento** — documentar requisitos e referências técnicas.
014. **endianness e alinhamento** — definir interfaces e invariantes do subsistema.
014. **endianness e alinhamento** — implementar o caminho básico funcional.
014. **endianness e alinhamento** — tratar limites, estados inválidos e erros.
014. **endianness e alinhamento** — cobrir operações normais com testes unitários.
014. **endianness e alinhamento** — adicionar casos extremos e regressões.
014. **endianness e alinhamento** — integrar com o barramento e o scheduler.
014. **endianness e alinhamento** — medir custo e corrigir gargalos sem alterar a semântica.
014. **endianness e alinhamento** — registrar diagnóstico, limitações e evidências.
014. **endianness e alinhamento** — executar validação de aceitação antes de marcar concluído.
015. **decodificador de endereços** — documentar requisitos e referências técnicas.
015. **decodificador de endereços** — definir interfaces e invariantes do subsistema.
015. **decodificador de endereços** — implementar o caminho básico funcional.
015. **decodificador de endereços** — tratar limites, estados inválidos e erros.
015. **decodificador de endereços** — cobrir operações normais com testes unitários.
015. **decodificador de endereços** — adicionar casos extremos e regressões.
015. **decodificador de endereços** — integrar com o barramento e o scheduler.
015. **decodificador de endereços** — medir custo e corrigir gargalos sem alterar a semântica.
015. **decodificador de endereços** — registrar diagnóstico, limitações e evidências.
015. **decodificador de endereços** — executar validação de aceitação antes de marcar concluído.
016. **registradores MMIO por dispositivo** — documentar requisitos e referências técnicas.
016. **registradores MMIO por dispositivo** — definir interfaces e invariantes do subsistema.
016. **registradores MMIO por dispositivo** — implementar o caminho básico funcional.
016. **registradores MMIO por dispositivo** — tratar limites, estados inválidos e erros.
016. **registradores MMIO por dispositivo** — cobrir operações normais com testes unitários.
016. **registradores MMIO por dispositivo** — adicionar casos extremos e regressões.
016. **registradores MMIO por dispositivo** — integrar com o barramento e o scheduler.
016. **registradores MMIO por dispositivo** — medir custo e corrigir gargalos sem alterar a semântica.
016. **registradores MMIO por dispositivo** — registrar diagnóstico, limitações e evidências.
016. **registradores MMIO por dispositivo** — executar validação de aceitação antes de marcar concluído.
017. **arbitragem e contenção** — documentar requisitos e referências técnicas.
017. **arbitragem e contenção** — definir interfaces e invariantes do subsistema.
017. **arbitragem e contenção** — implementar o caminho básico funcional.
017. **arbitragem e contenção** — tratar limites, estados inválidos e erros.
017. **arbitragem e contenção** — cobrir operações normais com testes unitários.
017. **arbitragem e contenção** — adicionar casos extremos e regressões.
017. **arbitragem e contenção** — integrar com o barramento e o scheduler.
017. **arbitragem e contenção** — medir custo e corrigir gargalos sem alterar a semântica.
017. **arbitragem e contenção** — registrar diagnóstico, limitações e evidências.
017. **arbitragem e contenção** — executar validação de aceitação antes de marcar concluído.
018. **SCU e interrupções** — documentar requisitos e referências técnicas.
018. **SCU e interrupções** — definir interfaces e invariantes do subsistema.
018. **SCU e interrupções** — implementar o caminho básico funcional.
018. **SCU e interrupções** — tratar limites, estados inválidos e erros.
018. **SCU e interrupções** — cobrir operações normais com testes unitários.
018. **SCU e interrupções** — adicionar casos extremos e regressões.
018. **SCU e interrupções** — integrar com o barramento e o scheduler.
018. **SCU e interrupções** — medir custo e corrigir gargalos sem alterar a semântica.
018. **SCU e interrupções** — registrar diagnóstico, limitações e evidências.
018. **SCU e interrupções** — executar validação de aceitação antes de marcar concluído.
019. **DMA origem/destino/tamanho** — documentar requisitos e referências técnicas.
019. **DMA origem/destino/tamanho** — definir interfaces e invariantes do subsistema.
019. **DMA origem/destino/tamanho** — implementar o caminho básico funcional.
019. **DMA origem/destino/tamanho** — tratar limites, estados inválidos e erros.
019. **DMA origem/destino/tamanho** — cobrir operações normais com testes unitários.
019. **DMA origem/destino/tamanho** — adicionar casos extremos e regressões.
019. **DMA origem/destino/tamanho** — integrar com o barramento e o scheduler.
019. **DMA origem/destino/tamanho** — medir custo e corrigir gargalos sem alterar a semântica.
019. **DMA origem/destino/tamanho** — registrar diagnóstico, limitações e evidências.
019. **DMA origem/destino/tamanho** — executar validação de aceitação antes de marcar concluído.
020. **testes e integração de barramento** — documentar requisitos e referências técnicas.
020. **testes e integração de barramento** — definir interfaces e invariantes do subsistema.
020. **testes e integração de barramento** — implementar o caminho básico funcional.
020. **testes e integração de barramento** — tratar limites, estados inválidos e erros.
020. **testes e integração de barramento** — cobrir operações normais com testes unitários.
020. **testes e integração de barramento** — adicionar casos extremos e regressões.
020. **testes e integração de barramento** — integrar com o barramento e o scheduler.
020. **testes e integração de barramento** — medir custo e corrigir gargalos sem alterar a semântica.
020. **testes e integração de barramento** — registrar diagnóstico, limitações e evidências.
020. **testes e integração de barramento** — executar validação de aceitação antes de marcar concluído.
## Bloco 1 — VDP1, VDP2, composição gráfica e sincronização
021. **comandos e listas VDP1** — documentar requisitos e referências técnicas.
021. **comandos e listas VDP1** — definir interfaces e invariantes do subsistema.
021. **comandos e listas VDP1** — implementar o caminho básico funcional.
021. **comandos e listas VDP1** — tratar limites, estados inválidos e erros.
021. **comandos e listas VDP1** — cobrir operações normais com testes unitários.
021. **comandos e listas VDP1** — adicionar casos extremos e regressões.
021. **comandos e listas VDP1** — integrar com o barramento e o scheduler.
021. **comandos e listas VDP1** — medir custo e corrigir gargalos sem alterar a semântica.
021. **comandos e listas VDP1** — registrar diagnóstico, limitações e evidências.
021. **comandos e listas VDP1** — executar validação de aceitação antes de marcar concluído.
022. **sprites e texturas** — documentar requisitos e referências técnicas.
022. **sprites e texturas** — definir interfaces e invariantes do subsistema.
022. **sprites e texturas** — implementar o caminho básico funcional.
022. **sprites e texturas** — tratar limites, estados inválidos e erros.
022. **sprites e texturas** — cobrir operações normais com testes unitários.
022. **sprites e texturas** — adicionar casos extremos e regressões.
022. **sprites e texturas** — integrar com o barramento e o scheduler.
022. **sprites e texturas** — medir custo e corrigir gargalos sem alterar a semântica.
022. **sprites e texturas** — registrar diagnóstico, limitações e evidências.
022. **sprites e texturas** — executar validação de aceitação antes de marcar concluído.
023. **polígonos e linhas** — documentar requisitos e referências técnicas.
023. **polígonos e linhas** — definir interfaces e invariantes do subsistema.
023. **polígonos e linhas** — implementar o caminho básico funcional.
023. **polígonos e linhas** — tratar limites, estados inválidos e erros.
023. **polígonos e linhas** — cobrir operações normais com testes unitários.
023. **polígonos e linhas** — adicionar casos extremos e regressões.
023. **polígonos e linhas** — integrar com o barramento e o scheduler.
023. **polígonos e linhas** — medir custo e corrigir gargalos sem alterar a semântica.
023. **polígonos e linhas** — registrar diagnóstico, limitações e evidências.
023. **polígonos e linhas** — executar validação de aceitação antes de marcar concluído.
024. **clipping e coordenadas** — documentar requisitos e referências técnicas.
024. **clipping e coordenadas** — definir interfaces e invariantes do subsistema.
024. **clipping e coordenadas** — implementar o caminho básico funcional.
024. **clipping e coordenadas** — tratar limites, estados inválidos e erros.
024. **clipping e coordenadas** — cobrir operações normais com testes unitários.
024. **clipping e coordenadas** — adicionar casos extremos e regressões.
024. **clipping e coordenadas** — integrar com o barramento e o scheduler.
024. **clipping e coordenadas** — medir custo e corrigir gargalos sem alterar a semântica.
024. **clipping e coordenadas** — registrar diagnóstico, limitações e evidências.
024. **clipping e coordenadas** — executar validação de aceitação antes de marcar concluído.
025. **framebuffer VDP1** — documentar requisitos e referências técnicas.
025. **framebuffer VDP1** — definir interfaces e invariantes do subsistema.
025. **framebuffer VDP1** — implementar o caminho básico funcional.
025. **framebuffer VDP1** — tratar limites, estados inválidos e erros.
025. **framebuffer VDP1** — cobrir operações normais com testes unitários.
025. **framebuffer VDP1** — adicionar casos extremos e regressões.
025. **framebuffer VDP1** — integrar com o barramento e o scheduler.
025. **framebuffer VDP1** — medir custo e corrigir gargalos sem alterar a semântica.
025. **framebuffer VDP1** — registrar diagnóstico, limitações e evidências.
025. **framebuffer VDP1** — executar validação de aceitação antes de marcar concluído.
026. **camadas VDP2** — documentar requisitos e referências técnicas.
026. **camadas VDP2** — definir interfaces e invariantes do subsistema.
026. **camadas VDP2** — implementar o caminho básico funcional.
026. **camadas VDP2** — tratar limites, estados inválidos e erros.
026. **camadas VDP2** — cobrir operações normais com testes unitários.
026. **camadas VDP2** — adicionar casos extremos e regressões.
026. **camadas VDP2** — integrar com o barramento e o scheduler.
026. **camadas VDP2** — medir custo e corrigir gargalos sem alterar a semântica.
026. **camadas VDP2** — registrar diagnóstico, limitações e evidências.
026. **camadas VDP2** — executar validação de aceitação antes de marcar concluído.
027. **scroll e planos** — documentar requisitos e referências técnicas.
027. **scroll e planos** — definir interfaces e invariantes do subsistema.
027. **scroll e planos** — implementar o caminho básico funcional.
027. **scroll e planos** — tratar limites, estados inválidos e erros.
027. **scroll e planos** — cobrir operações normais com testes unitários.
027. **scroll e planos** — adicionar casos extremos e regressões.
027. **scroll e planos** — integrar com o barramento e o scheduler.
027. **scroll e planos** — medir custo e corrigir gargalos sem alterar a semântica.
027. **scroll e planos** — registrar diagnóstico, limitações e evidências.
027. **scroll e planos** — executar validação de aceitação antes de marcar concluído.
028. **paletas e transparência** — documentar requisitos e referências técnicas.
028. **paletas e transparência** — definir interfaces e invariantes do subsistema.
028. **paletas e transparência** — implementar o caminho básico funcional.
028. **paletas e transparência** — tratar limites, estados inválidos e erros.
028. **paletas e transparência** — cobrir operações normais com testes unitários.
028. **paletas e transparência** — adicionar casos extremos e regressões.
028. **paletas e transparência** — integrar com o barramento e o scheduler.
028. **paletas e transparência** — medir custo e corrigir gargalos sem alterar a semântica.
028. **paletas e transparência** — registrar diagnóstico, limitações e evidências.
028. **paletas e transparência** — executar validação de aceitação antes de marcar concluído.
029. **prioridades e composição** — documentar requisitos e referências técnicas.
029. **prioridades e composição** — definir interfaces e invariantes do subsistema.
029. **prioridades e composição** — implementar o caminho básico funcional.
029. **prioridades e composição** — tratar limites, estados inválidos e erros.
029. **prioridades e composição** — cobrir operações normais com testes unitários.
029. **prioridades e composição** — adicionar casos extremos e regressões.
029. **prioridades e composição** — integrar com o barramento e o scheduler.
029. **prioridades e composição** — medir custo e corrigir gargalos sem alterar a semântica.
029. **prioridades e composição** — registrar diagnóstico, limitações e evidências.
029. **prioridades e composição** — executar validação de aceitação antes de marcar concluído.
030. **timing de quadro e testes gráficos** — documentar requisitos e referências técnicas.
030. **timing de quadro e testes gráficos** — definir interfaces e invariantes do subsistema.
030. **timing de quadro e testes gráficos** — implementar o caminho básico funcional.
030. **timing de quadro e testes gráficos** — tratar limites, estados inválidos e erros.
030. **timing de quadro e testes gráficos** — cobrir operações normais com testes unitários.
030. **timing de quadro e testes gráficos** — adicionar casos extremos e regressões.
030. **timing de quadro e testes gráficos** — integrar com o barramento e o scheduler.
030. **timing de quadro e testes gráficos** — medir custo e corrigir gargalos sem alterar a semântica.
030. **timing de quadro e testes gráficos** — registrar diagnóstico, limitações e evidências.
030. **timing de quadro e testes gráficos** — executar validação de aceitação antes de marcar concluído.
## Bloco 1 — CD Block, SCSP, SMPC e periféricos
031. **parser de imagem de disco** — documentar requisitos e referências técnicas.
031. **parser de imagem de disco** — definir interfaces e invariantes do subsistema.
031. **parser de imagem de disco** — implementar o caminho básico funcional.
031. **parser de imagem de disco** — tratar limites, estados inválidos e erros.
031. **parser de imagem de disco** — cobrir operações normais com testes unitários.
031. **parser de imagem de disco** — adicionar casos extremos e regressões.
031. **parser de imagem de disco** — integrar com o barramento e o scheduler.
031. **parser de imagem de disco** — medir custo e corrigir gargalos sem alterar a semântica.
031. **parser de imagem de disco** — registrar diagnóstico, limitações e evidências.
031. **parser de imagem de disco** — executar validação de aceitação antes de marcar concluído.
032. **TOC e faixas de CD** — documentar requisitos e referências técnicas.
032. **TOC e faixas de CD** — definir interfaces e invariantes do subsistema.
032. **TOC e faixas de CD** — implementar o caminho básico funcional.
032. **TOC e faixas de CD** — tratar limites, estados inválidos e erros.
032. **TOC e faixas de CD** — cobrir operações normais com testes unitários.
032. **TOC e faixas de CD** — adicionar casos extremos e regressões.
032. **TOC e faixas de CD** — integrar com o barramento e o scheduler.
032. **TOC e faixas de CD** — medir custo e corrigir gargalos sem alterar a semântica.
032. **TOC e faixas de CD** — registrar diagnóstico, limitações e evidências.
032. **TOC e faixas de CD** — executar validação de aceitação antes de marcar concluído.
033. **leitura de setores** — documentar requisitos e referências técnicas.
033. **leitura de setores** — definir interfaces e invariantes do subsistema.
033. **leitura de setores** — implementar o caminho básico funcional.
033. **leitura de setores** — tratar limites, estados inválidos e erros.
033. **leitura de setores** — cobrir operações normais com testes unitários.
033. **leitura de setores** — adicionar casos extremos e regressões.
033. **leitura de setores** — integrar com o barramento e o scheduler.
033. **leitura de setores** — medir custo e corrigir gargalos sem alterar a semântica.
033. **leitura de setores** — registrar diagnóstico, limitações e evidências.
033. **leitura de setores** — executar validação de aceitação antes de marcar concluído.
034. **comandos do CD Block** — documentar requisitos e referências técnicas.
034. **comandos do CD Block** — definir interfaces e invariantes do subsistema.
034. **comandos do CD Block** — implementar o caminho básico funcional.
034. **comandos do CD Block** — tratar limites, estados inválidos e erros.
034. **comandos do CD Block** — cobrir operações normais com testes unitários.
034. **comandos do CD Block** — adicionar casos extremos e regressões.
034. **comandos do CD Block** — integrar com o barramento e o scheduler.
034. **comandos do CD Block** — medir custo e corrigir gargalos sem alterar a semântica.
034. **comandos do CD Block** — registrar diagnóstico, limitações e evidências.
034. **comandos do CD Block** — executar validação de aceitação antes de marcar concluído.
035. **bootstrap BIOS e boot de mídia** — documentar requisitos e referências técnicas.
035. **bootstrap BIOS e boot de mídia** — definir interfaces e invariantes do subsistema.
035. **bootstrap BIOS e boot de mídia** — implementar o caminho básico funcional.
035. **bootstrap BIOS e boot de mídia** — tratar limites, estados inválidos e erros.
035. **bootstrap BIOS e boot de mídia** — cobrir operações normais com testes unitários.
035. **bootstrap BIOS e boot de mídia** — adicionar casos extremos e regressões.
035. **bootstrap BIOS e boot de mídia** — integrar com o barramento e o scheduler.
035. **bootstrap BIOS e boot de mídia** — medir custo e corrigir gargalos sem alterar a semântica.
035. **bootstrap BIOS e boot de mídia** — registrar diagnóstico, limitações e evidências.
035. **bootstrap BIOS e boot de mídia** — executar validação de aceitação antes de marcar concluído.
036. **canais SCSP** — documentar requisitos e referências técnicas.
036. **canais SCSP** — definir interfaces e invariantes do subsistema.
036. **canais SCSP** — implementar o caminho básico funcional.
036. **canais SCSP** — tratar limites, estados inválidos e erros.
036. **canais SCSP** — cobrir operações normais com testes unitários.
036. **canais SCSP** — adicionar casos extremos e regressões.
036. **canais SCSP** — integrar com o barramento e o scheduler.
036. **canais SCSP** — medir custo e corrigir gargalos sem alterar a semântica.
036. **canais SCSP** — registrar diagnóstico, limitações e evidências.
036. **canais SCSP** — executar validação de aceitação antes de marcar concluído.
037. **amostras e formatos de áudio** — documentar requisitos e referências técnicas.
037. **amostras e formatos de áudio** — definir interfaces e invariantes do subsistema.
037. **amostras e formatos de áudio** — implementar o caminho básico funcional.
037. **amostras e formatos de áudio** — tratar limites, estados inválidos e erros.
037. **amostras e formatos de áudio** — cobrir operações normais com testes unitários.
037. **amostras e formatos de áudio** — adicionar casos extremos e regressões.
037. **amostras e formatos de áudio** — integrar com o barramento e o scheduler.
037. **amostras e formatos de áudio** — medir custo e corrigir gargalos sem alterar a semântica.
037. **amostras e formatos de áudio** — registrar diagnóstico, limitações e evidências.
037. **amostras e formatos de áudio** — executar validação de aceitação antes de marcar concluído.
038. **mixagem e temporização** — documentar requisitos e referências técnicas.
038. **mixagem e temporização** — definir interfaces e invariantes do subsistema.
038. **mixagem e temporização** — implementar o caminho básico funcional.
038. **mixagem e temporização** — tratar limites, estados inválidos e erros.
038. **mixagem e temporização** — cobrir operações normais com testes unitários.
038. **mixagem e temporização** — adicionar casos extremos e regressões.
038. **mixagem e temporização** — integrar com o barramento e o scheduler.
038. **mixagem e temporização** — medir custo e corrigir gargalos sem alterar a semântica.
038. **mixagem e temporização** — registrar diagnóstico, limitações e evidências.
038. **mixagem e temporização** — executar validação de aceitação antes de marcar concluído.
039. **SMPC e controle Saturn** — documentar requisitos e referências técnicas.
039. **SMPC e controle Saturn** — definir interfaces e invariantes do subsistema.
039. **SMPC e controle Saturn** — implementar o caminho básico funcional.
039. **SMPC e controle Saturn** — tratar limites, estados inválidos e erros.
039. **SMPC e controle Saturn** — cobrir operações normais com testes unitários.
039. **SMPC e controle Saturn** — adicionar casos extremos e regressões.
039. **SMPC e controle Saturn** — integrar com o barramento e o scheduler.
039. **SMPC e controle Saturn** — medir custo e corrigir gargalos sem alterar a semântica.
039. **SMPC e controle Saturn** — registrar diagnóstico, limitações e evidências.
039. **SMPC e controle Saturn** — executar validação de aceitação antes de marcar concluído.
040. **periféricos, IRQ e testes** — documentar requisitos e referências técnicas.
040. **periféricos, IRQ e testes** — definir interfaces e invariantes do subsistema.
040. **periféricos, IRQ e testes** — implementar o caminho básico funcional.
040. **periféricos, IRQ e testes** — tratar limites, estados inválidos e erros.
040. **periféricos, IRQ e testes** — cobrir operações normais com testes unitários.
040. **periféricos, IRQ e testes** — adicionar casos extremos e regressões.
040. **periféricos, IRQ e testes** — integrar com o barramento e o scheduler.
040. **periféricos, IRQ e testes** — medir custo e corrigir gargalos sem alterar a semântica.
040. **periféricos, IRQ e testes** — registrar diagnóstico, limitações e evidências.
040. **periféricos, IRQ e testes** — executar validação de aceitação antes de marcar concluído.
## Bloco 1 — Integração, interface, biblioteca, saves e build Xenon
041. **integração dos dois SH-2** — documentar requisitos e referências técnicas.
041. **integração dos dois SH-2** — definir interfaces e invariantes do subsistema.
041. **integração dos dois SH-2** — implementar o caminho básico funcional.
041. **integração dos dois SH-2** — tratar limites, estados inválidos e erros.
041. **integração dos dois SH-2** — cobrir operações normais com testes unitários.
041. **integração dos dois SH-2** — adicionar casos extremos e regressões.
041. **integração dos dois SH-2** — integrar com o barramento e o scheduler.
041. **integração dos dois SH-2** — medir custo e corrigir gargalos sem alterar a semântica.
041. **integração dos dois SH-2** — registrar diagnóstico, limitações e evidências.
041. **integração dos dois SH-2** — executar validação de aceitação antes de marcar concluído.
042. **scheduler e sincronização global** — documentar requisitos e referências técnicas.
042. **scheduler e sincronização global** — definir interfaces e invariantes do subsistema.
042. **scheduler e sincronização global** — implementar o caminho básico funcional.
042. **scheduler e sincronização global** — tratar limites, estados inválidos e erros.
042. **scheduler e sincronização global** — cobrir operações normais com testes unitários.
042. **scheduler e sincronização global** — adicionar casos extremos e regressões.
042. **scheduler e sincronização global** — integrar com o barramento e o scheduler.
042. **scheduler e sincronização global** — medir custo e corrigir gargalos sem alterar a semântica.
042. **scheduler e sincronização global** — registrar diagnóstico, limitações e evidências.
042. **scheduler e sincronização global** — executar validação de aceitação antes de marcar concluído.
043. **integração VDP1/VDP2** — documentar requisitos e referências técnicas.
043. **integração VDP1/VDP2** — definir interfaces e invariantes do subsistema.
043. **integração VDP1/VDP2** — implementar o caminho básico funcional.
043. **integração VDP1/VDP2** — tratar limites, estados inválidos e erros.
043. **integração VDP1/VDP2** — cobrir operações normais com testes unitários.
043. **integração VDP1/VDP2** — adicionar casos extremos e regressões.
043. **integração VDP1/VDP2** — integrar com o barramento e o scheduler.
043. **integração VDP1/VDP2** — medir custo e corrigir gargalos sem alterar a semântica.
043. **integração VDP1/VDP2** — registrar diagnóstico, limitações e evidências.
043. **integração VDP1/VDP2** — executar validação de aceitação antes de marcar concluído.
044. **integração CD/SCSP/SMPC** — documentar requisitos e referências técnicas.
044. **integração CD/SCSP/SMPC** — definir interfaces e invariantes do subsistema.
044. **integração CD/SCSP/SMPC** — implementar o caminho básico funcional.
044. **integração CD/SCSP/SMPC** — tratar limites, estados inválidos e erros.
044. **integração CD/SCSP/SMPC** — cobrir operações normais com testes unitários.
044. **integração CD/SCSP/SMPC** — adicionar casos extremos e regressões.
044. **integração CD/SCSP/SMPC** — integrar com o barramento e o scheduler.
044. **integração CD/SCSP/SMPC** — medir custo e corrigir gargalos sem alterar a semântica.
044. **integração CD/SCSP/SMPC** — registrar diagnóstico, limitações e evidências.
044. **integração CD/SCSP/SMPC** — executar validação de aceitação antes de marcar concluído.
045. **interface principal** — documentar requisitos e referências técnicas.
045. **interface principal** — definir interfaces e invariantes do subsistema.
045. **interface principal** — implementar o caminho básico funcional.
045. **interface principal** — tratar limites, estados inválidos e erros.
045. **interface principal** — cobrir operações normais com testes unitários.
045. **interface principal** — adicionar casos extremos e regressões.
045. **interface principal** — integrar com o barramento e o scheduler.
045. **interface principal** — medir custo e corrigir gargalos sem alterar a semântica.
045. **interface principal** — registrar diagnóstico, limitações e evidências.
045. **interface principal** — executar validação de aceitação antes de marcar concluído.
046. **scanner de ROMs e metadados** — documentar requisitos e referências técnicas.
046. **scanner de ROMs e metadados** — definir interfaces e invariantes do subsistema.
046. **scanner de ROMs e metadados** — implementar o caminho básico funcional.
046. **scanner de ROMs e metadados** — tratar limites, estados inválidos e erros.
046. **scanner de ROMs e metadados** — cobrir operações normais com testes unitários.
046. **scanner de ROMs e metadados** — adicionar casos extremos e regressões.
046. **scanner de ROMs e metadados** — integrar com o barramento e o scheduler.
046. **scanner de ROMs e metadados** — medir custo e corrigir gargalos sem alterar a semântica.
046. **scanner de ROMs e metadados** — registrar diagnóstico, limitações e evidências.
046. **scanner de ROMs e metadados** — executar validação de aceitação antes de marcar concluído.
047. **capas, favoritos e filtros** — documentar requisitos e referências técnicas.
047. **capas, favoritos e filtros** — definir interfaces e invariantes do subsistema.
047. **capas, favoritos e filtros** — implementar o caminho básico funcional.
047. **capas, favoritos e filtros** — tratar limites, estados inválidos e erros.
047. **capas, favoritos e filtros** — cobrir operações normais com testes unitários.
047. **capas, favoritos e filtros** — adicionar casos extremos e regressões.
047. **capas, favoritos e filtros** — integrar com o barramento e o scheduler.
047. **capas, favoritos e filtros** — medir custo e corrigir gargalos sem alterar a semântica.
047. **capas, favoritos e filtros** — registrar diagnóstico, limitações e evidências.
047. **capas, favoritos e filtros** — executar validação de aceitação antes de marcar concluído.
048. **save state e save data** — documentar requisitos e referências técnicas.
048. **save state e save data** — definir interfaces e invariantes do subsistema.
048. **save state e save data** — implementar o caminho básico funcional.
048. **save state e save data** — tratar limites, estados inválidos e erros.
048. **save state e save data** — cobrir operações normais com testes unitários.
048. **save state e save data** — adicionar casos extremos e regressões.
048. **save state e save data** — integrar com o barramento e o scheduler.
048. **save state e save data** — medir custo e corrigir gargalos sem alterar a semântica.
048. **save state e save data** — registrar diagnóstico, limitações e evidências.
048. **save state e save data** — executar validação de aceitação antes de marcar concluído.
049. **toolchain ELF32 e default.xex** — documentar requisitos e referências técnicas.
049. **toolchain ELF32 e default.xex** — definir interfaces e invariantes do subsistema.
049. **toolchain ELF32 e default.xex** — implementar o caminho básico funcional.
049. **toolchain ELF32 e default.xex** — tratar limites, estados inválidos e erros.
049. **toolchain ELF32 e default.xex** — cobrir operações normais com testes unitários.
049. **toolchain ELF32 e default.xex** — adicionar casos extremos e regressões.
049. **toolchain ELF32 e default.xex** — integrar com o barramento e o scheduler.
049. **toolchain ELF32 e default.xex** — medir custo e corrigir gargalos sem alterar a semântica.
049. **toolchain ELF32 e default.xex** — registrar diagnóstico, limitações e evidências.
049. **toolchain ELF32 e default.xex** — executar validação de aceitação antes de marcar concluído.
050. **CI, artefatos e validação em hardware** — documentar requisitos e referências técnicas.
050. **CI, artefatos e validação em hardware** — definir interfaces e invariantes do subsistema.
050. **CI, artefatos e validação em hardware** — implementar o caminho básico funcional.
050. **CI, artefatos e validação em hardware** — tratar limites, estados inválidos e erros.
050. **CI, artefatos e validação em hardware** — cobrir operações normais com testes unitários.
050. **CI, artefatos e validação em hardware** — adicionar casos extremos e regressões.
050. **CI, artefatos e validação em hardware** — integrar com o barramento e o scheduler.
050. **CI, artefatos e validação em hardware** — medir custo e corrigir gargalos sem alterar a semântica.
050. **CI, artefatos e validação em hardware** — registrar diagnóstico, limitações e evidências.
050. **CI, artefatos e validação em hardware** — executar validação de aceitação antes de marcar concluído.

## Critérios globais de aceitação
1. Testes host com avisos tratados como erros e regressões anteriores preservadas.
2. Build Xenon ELF32 e pacote `default.xex` gerados no workflow.
3. Artefatos publicados com logs/checksums e ligação ao commit de origem.
4. Nenhuma alegação de boot físico ou jogo compatível sem evidência de teste em console.
5. Cada subsistema permanece marcado como parcial até implementar comportamento real específico do hardware Saturn.
