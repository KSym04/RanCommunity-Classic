/*******************************************************************************************
 RAN ONLINE SERVER SKILL EFFECT & VISUAL LOAD INVESTIGATION
 Target Problem:
 - Lag occurs when many skill effects are active
 - Lag worsens with AoE, buffs, and stacked visuals
 - Player count is low
 - Hardware and tick time are not limiting factors

 Core Insight:
 - Skill "visuals" often trigger server-side work:
   state updates, buff logic, hit checks, broadcasts

 Copilot Role:
 - Audit server-side skill execution paths
 - Separate cosmetic effects from authoritative logic
 - Identify packet storms and redundant processing

 ==========================================================================================
 STEP 1: SKILL EXECUTION PIPELINE
 ==========================================================================================

 1. Locate the entry point for skill usage.
 2. Trace the full execution path:
    - Skill request validation
    - Cooldown checks
    - Target resolution
    - Damage calculation
    - Buff application
    - Effect trigger
    - Network broadcast

 Flag as CRITICAL if:
 - Skill visuals are triggered server-side per target
 - Damage or buff logic is coupled to visual effect logic
 - Skill logic loops per target per tick

 Output:
 - Full skill pipeline map
 - Identification of cosmetic vs authoritative steps

 ==========================================================================================
 STEP 2: EFFECT SPAWN & LIFETIME MANAGEMENT
 ==========================================================================================

 1. Locate code responsible for:
    - Effect creation
    - Effect lifetime tracking
    - Effect destruction

 Flag as CRITICAL if:
 - Effect objects are created with new/delete
 - Effects are updated every tick
 - Effects are stored in global lists
 - Effects have server-side collision or timers

 Required Fix Pattern:
 - Server triggers effect once
 - Client owns lifetime and animation
 - No per-tick server updates for visuals

 Output:
 - Effect object count
 - Lifetime management issues

 ==========================================================================================
 STEP 3: BUFF / DEBUFF VISUAL COUPLING
 ==========================================================================================

 1. Inspect buff application code.
 2. Trace how buff visuals are triggered.

 Flag as CRITICAL if:
 - Buff visual triggered per tick
 - Buff re-sent every update
 - Buff packets include full state repeatedly

 Required Fix Pattern:
 - Send buff start
 - Send buff end
 - Never re-send unchanged buff state

 Output:
 - Buff packet frequency
 - Redundant state transmission

 ==========================================================================================
 STEP 4: AOE EFFECT MULTIPLICATION
 ==========================================================================================

 1. Inspect AoE skill effect logic.
 2. Trace loops involving:
    - Targets
    - Effects
    - Packets

 Flag as CRITICAL if:
 - Effects are spawned per target
 - Effect packets sent per target
 - Effect data duplicated per hit

 Required Fix Pattern:
 - One AoE effect origin
 - Clients derive visuals locally
 - Batch hit results

 Output:
 - AoE effect complexity
 - Packet amplification estimate

 ==========================================================================================
 STEP 5: NETWORK PACKET STORM ANALYSIS
 ==========================================================================================

 1. Identify packets related to:
    - Skill cast
    - Effect spawn
    - Hit confirmation
    - Buff updates

 Flag as CRITICAL if:
 - Multiple packets sent for a single skill
 - Same data sent to same client repeatedly
 - Packets sent to uninterested clients

 Required Fix Pattern:
 - Aggregate packets
 - Interest-based recipients
 - Delta-only updates

 Output:
 - Packets per skill cast
 - Reduction opportunities

 ==========================================================================================
 STEP 6: SKILL TIMERS & PERIODIC EFFECTS
 ==========================================================================================

 1. Inspect DOT, HOT, channeling, and aura skills.
 2. Identify how timers are processed.

 Flag as CRITICAL if:
 - Timers tick every server tick
 - Damage calculated every tick
 - Visual updates every tick

 Required Fix Pattern:
 - Scheduled execution
 - Fixed interval ticks (e.g. 500 ms)
 - Client-driven visuals

 Output:
 - Timer execution cost
 - Scheduler improvement plan

 ==========================================================================================
 STEP 7: DEBUG, VALIDATION, AND LOGGING
 ==========================================================================================

 Flag as CRITICAL if:
 - Skill debug logs
 - Anti-cheat logs tied to skills
 - Validation logs inside loops

 Output:
 - All skill-related logs
 - Removal or gating plan

 ==========================================================================================
 FINAL OUTPUT REQUIREMENTS
 ==========================================================================================

 Copilot must output:
 - Top skill-related lag sources
 - Exact file names and line numbers
 - Explanation of packet amplification
 - Concrete refactor suggestions

 Do NOT:
 - Suggest disabling effects as a solution
 - Suggest reducing skill usage
 - Suggest lowering tick time

 END OF INSTRUCTION
*******************************************************************************************/
