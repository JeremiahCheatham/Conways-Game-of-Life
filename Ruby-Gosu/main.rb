require("gosu")

WIDTH = 800
HEIGHT = 600
SIZE = 9

class Game < Gosu::Window
    def initialize
        @width = WIDTH
        @height = HEIGHT
        @rows = (@height / SIZE).floor
        @columns = (@width / SIZE).floor
        @size = SIZE - 1
        @delay_target = 16
        @delay_count = 1
        @pause = false

        super(@width, @height)

        self.caption = "Game of Life"

        @font = Gosu::Font.new(30)

        @board = []
        @new_board = []
        @rows.times do
            column = []
            new_column = []
            @columns.times do
                column << rand(2)
                new_column << 0
            end
            @board << column
            @new_board << new_column
        end
    end

    def update
        if @delay_count >= @delay_target and !@pause
            @board.each_index do |row|
                @board[row].each_index do |column|
                    count = 0
                    [row - 1, row, row + 1].each do |y|
                        [column - 1, column, column + 1].each do |x|
                            unless row == y and column == x
                                count += @board[y % @rows][x % @columns]
                            end
                        end
                    end
                    if @board[row][column] == 1 and count > 1 and count < 4
                        @new_board[row][column] = 1
                    elsif @board[row][column] == 0 and count == 3
                        @new_board[row][column] = 1
                    else
                        @new_board[row][column] = 0
                    end
                end
            end
            board = @board
            @board = @new_board
            @new_board = board
            @delay_count = 1
        else
            @delay_count += 1
        end
    end

    def draw
        Gosu.draw_rect(0, 0, @width, @height, Gosu::Color::WHITE)
        @board.each_index do |row|
            @board[row].each_index do |column|
                if @board[row][column] == 1
                    Gosu.draw_rect(column * SIZE, row * SIZE, @size, @size, Gosu::Color::GRAY)
                end
            end
        end
        if @pause
            @font.draw_markup("<b>Paused</b>", 10, 10, 0, scale_x = 1, scale_y = 1, color = Gosu::Color::BLACK)
        else
            @font.draw_markup("<b>Speed: #{8.0 / @delay_target}</b>", 10, 10, 0, scale_x = 1, scale_y = 1, color = Gosu::Color::BLACK)
        end
    end

    def button_down(id)
        case id
        when Gosu::KB_ESCAPE
            close()
        when Gosu::KB_DOWN
            if @delay_target < 64
                @delay_target *= 2
            end
        when Gosu::KB_UP
            if @delay_target > 1
                @delay_target /= 2
            end
        when Gosu::KB_SPACE
            @pause = @pause ? false : true
        when Gosu::MsLeft
            row = (mouse_y / SIZE).floor
            column = (mouse_x / SIZE).floor
            @board[row][column] = @board[row][column] == 1 ? 0 : 1
        when Gosu::KB_C
            @board.each_index do |row|
                @board[row].each_index do |column|
                    @board[row][column] = 0
                end
            end
        when Gosu::KB_R
            @board.each_index do |row|
                @board[row].each_index do |column|
                    @board[row][column] = rand(2)
                end
            end
        end
    end
end

Game.new.show()