# Covert-Channel Busted

These results were collected early in my PhD thesis while exploring multiple ideas without a clear objective in mind (i.e., open-source and reproducible research). As a result, this code is not as well-structured and lacks reproducible scripts compared to the remaining experiments. Nevertheless, users will find all the necessary code and infrastructure to (manually) replicate the experiments and measure the cache covert-channels. I do not intend to improve this code further; it will remain as it is. Additionally, the TF-M version is quite outdated compared to the one used in the other experiments (v1.4 vs v2.2), making it not directly portable to newer TF-M versions (the main reason I did not invest time in porting it). Nonetheless, the core covert-channel implementation should remain intact; only the scaffolding code will change. 

|   Board       | CPU |  ICache Size    | Note                                                                    |
|:---           |:---:|:---:            | :---                                                                    |
| MuscaB1       | M33 |  2KB            | Working with TF-M configured in PSA Level 3                             |
| nRF9160       | M33 |  2KB            | Working with TF-M configured in PSA Level 2                             |
| STM32F767     | M4  |  16KB           | Baremetal (F7 doesn't support TZ)                                       |
| STM32L552     | M33 |  8KB            | Working with TF-M configured in PSA Level 2                             |
| Nuvoton M2351 | M23 |  4KB            | I've lost the code `¯\_(ツ)_/¯`, but the logic is similar to the remaining |
| NXP LCP55S69  | M33 |  128B           | I've lost the code `¯\_(ツ)_/¯`, but the logic is similar to the remaining |