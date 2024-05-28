def Menu(Address):
    print('Welcome to the DIY Tea & Juice Maker!')

    # Choose the base
    base_msg = 'What kind of base do you want?'
    base_kind = int(input(base_msg + ' Please enter 1 for milk or 2 for fruit: '))

    if base_kind == 1:  # Milk base was chosen
        lac_msg = 'Do you have lactose intolerance?'
        lactose_intolerance = input(lac_msg + ' y for yes, n for no: ')
        if lactose_intolerance == 'y':
            milk_msg = 'Do you want soy milk or oat milk? Please type in'
            base_type = input(milk_msg + ' your choice [soy/oat]: ') + ' milk'
        else:
            base_type = 'regular milk'

    else:  # Fruit base was chosen
        fruit_msg = 'Which fruit do you want? Please type in'
        fruit = input(fruit_msg + ' your choice [mango/strawberry]: ')
        base_type = fruit + ' juice'

    # Choose the tea
    tea_msg = 'From the following tea type:\n- No Tea\n- Black Tea\n- Green Tea\n- Matcha'
    tea_type = input(tea_msg + ' \nPlease choose a tea type: ')

    if base_kind != 1 and tea_type == 'Matcha':
        print('Invalid choice! End of the program')  # Can't have Matcha with fruit
        return

    # Choose topping
    topping_msg = 'From the following toppings:\n- No Topping\n- Bobas\n- Coconut Jelly'
    topping = input(topping_msg + '\nPlease enter your choice for toppings: ')

    # Choose beverage size
    size_msg = 'Please enter your desired size of cup'
    '(Please enter s for small, m for medium, or l for large)'
    cup_size = input(size_msg + ' : ')
    if cup_size == 's':
        m_drink = 355
    elif cup_size == 'm':
        m_drink = 473
    else:
        m_drink = 621

    # Choose beverage temperature
    temp_msg = 'Please enter your desired temperature of your beverage'
    desired_temp = float(input(temp_msg + ' (between 1 and 4 degrees): '))
    if desired_temp < 1 or desired_temp > 10:
        print('Invalid choice! End of the program')
        return

    # Calculate ice cubes needed
    else:
        if desired_temp > 4:
            desired_temp = 4.0  # Use float instead of integer.
        C_DRINK: float = 4.184  # Heat capacity
        H_FUSION: int = 334  # Heat fusion
        temp_diff = 25 - desired_temp  # Drink temperature difference
        ice_cubes = int((m_drink * C_DRINK * temp_diff)
                        / (H_FUSION + C_DRINK * desired_temp) / 5)

        # Result
        print('Your drink is a', base_type, 'and', tea_type,
              'with', topping + '.')
        print('The temperature of your beverage will be', desired_temp,
              'Celcius degree after all', ice_cubes, 'ice cubes melted.')
        print('It will be delivered to', Address)
