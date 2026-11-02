#### 2025_11~12월 언리얼 프로젝트\
프로젝트 이름: 크라임엣지\
탐정이 되어서 피해자를 죽인 범인을 찾는 게임(플레이 타임: 15분)\
모티브:(넷플릭스 오리지널 예능- 크X임씬 제로),(시그널), (게임- 카리마라)\
개발 엔진 : Unreal Engine_버전: 5.6.x

####주요 스크립트 구성
<img width="1468" height="823" alt="image" src="https://github.com/user-attachments/assets/8e6dc1fc-87d7-4325-a996-c57e6ec1b706" />

<img width="1550" height="831" alt="image" src="https://github.com/user-attachments/assets/c4ea04ce-2d42-4d7f-9ce0-0db17a36e99f" />

<img width="1548" height="828" alt="image" src="https://github.com/user-attachments/assets/48064933-33cd-4ed3-be8f-ee7d6a005c01" />

<img width="1548" height="828" alt="image" src="https://github.com/user-attachments/assets/601d5407-bb62-4f55-8f24-b9d55e3fb304" />

<img width="1546" height="826" alt="image" src="https://github.com/user-attachments/assets/4602b008-f9d5-40f9-ae19-7075df8bb39d" />
-이부분의 추가적인 설명 → 지금 현재 증거 이름이 text로 했는데 그부분과 MPC제출Table을 일치를 시키기 위해 “_”를 추가 하여 일치하면 성공 TEXT를 보냄

gameInstance.cpp
게임 tp 위치를 저장하고, 날짜가 증가시 텔포 위치를 바꾸고 NPC를 교체한다

BaseNPC.h
엠피씨의 대화 호출 _블루프린트와 연결되어 있음

MyPlayer.cpp
캐릭터 이동과 카메라 컴포넌트를 이용해서 1인칭 설정을 하며, 키 입력을 통해 Line Trace을 수행하여 증거 배열에 있는 것들이 입력되면 해당 증거가 true가 되어 인벤토리 창에 나타나게 되며,획득된 아이템은 다시 획득되지 않습니다.
만약 Line Trace가 NPC이면 대화를 시작하며, InitiateDialogue을 시작합니다.

<img width="389" height="416" alt="image" src="https://github.com/user-attachments/assets/7e5373bf-9782-4fa8-997f-a8154d9f8310" />


외부 에셋 :\
Isometric - Interiors | Fab - 원룸3곳\
믹사모  → MPC앉는 모션들\
BGM:\
Miserable(미저러블) | 음악 | 공유 마당_20의 여름날 | 음악 | 공유 마당\
A테마:    GLADIATOR | 음악 | 공유 마당\
B테마:    우산으로 떨어지는 빗소리 | 음악 | 공유 마당 →밖에 비오는 모습으로\
C테마:    Wind drop | 음악 | 공유 마당\
탐사 시간 : 미스터미스테리 | 음악 | 공유 마당\
[노션 주소](https://harmless-page-b10.notion.site/2a047f93dcd48068b4c2f516d6025d92?v=2a047f93dcd480b2bc97000c680ef264&pvs=74)
